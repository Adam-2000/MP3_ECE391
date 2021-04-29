#include "system_calls.h"
#include "lib.h"
/*
void system_call_func(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3){
    printf("system calls:%d, %d, %d, %d\n", id, arg1, arg2, arg3);
    return;
}
*/

// void system_call_func(){
//     printf("system calls\n");
//     return;
// }
// function pointer tables for different types
static fop_table_t fop_table_file = {
    file_read, file_write, file_open, file_close
};
static fop_table_t fop_table_directory = {
    directory_read, directory_write, directory_open, directory_close
};
static fop_table_t fop_table_rtc = {
    rtc_read, rtc_write, rtc_open, rtc_close
};
// static fop_table_t fop_table_terminal = {
//     terminal_read, terminal_write, terminal_open, terminal_close
// };
// pcb busy(1)/free(0) array
static int process_number_array[MAX_PROCESS_NUMBER];
// current process number
int cur_process_number;
// current pcb pointer
static pcb_t* pcb_ptr;

static uint8_t arg_buffer[ARG_BUF_SIZE];
/*
 *	Function: get_pcb_ptr
 *	Description: get pcb pointer
 *	inputs:		none
 *	outputs:	pcb pointer
 */
uint32_t get_pcb_ptr(){
    return (uint32_t)pcb_ptr;
}
void set_pcb_ptr(pcb_t* new_pcb_ptr){
    pcb_ptr = new_pcb_ptr;
}

/*
 *	Function: system_calls_init
 *	Description: initialize system calls
 *	inputs:		none
 *	outputs:	none
 */
void system_calls_init(){
    int i;
    process_number_array[0] = 1;
    for (i = 1; i < MAX_PROCESS_NUMBER; i++){
        process_number_array[i] = 0;
    }
    cur_process_number = 0;
    pcb_ptr = (pcb_t*) (KERNEL_END - KERNEL_STACK_WIDTH);
    pcb_t new_pcb;
    memcpy(pcb_ptr, &new_pcb, sizeof(pcb_t));  
    //set_paging_directory(KERNEL_END + cur_process_number * PAGE_SIZE_BIG);
}


/*
 *	Function: execute_helper
 *	Description: helper function of execute system call
 *	inputs:		command -- execute command
 *	outputs:	-1 if fail
 *              256 if exceptions
 *              others with other meaning
 *  Side effects: create a pcb and prepare for context switch
 */
int32_t execute_helper(const uint8_t* command){
    //printf("IN FUNCTION: EXECUTE_HELPER, %s\n", command);
    int ret;
    int i;
    dentry_t tem_dentry;
    uint8_t filename[FILENAMESIZE + 1];
    uint32_t magic = 0;
    int32_t virtual_addr_1instr;
    int new_process_number;
    pcb_t* new_pcb_ptr;
    // get rid of space before
    while (*command == ' '){
        command++;
    }
    // get args for next time, first to read the filename
    for (i = 0; i <= FILENAMESIZE; i++, command++){
        if (*command == ' ' || *command == '\0' || *command == '\n'){
            filename[i] = '\0';
            break;
        }
        filename[i] = *command;
    }
    if (i == FILENAMESIZE + 1){
        printf("command filename might be too long: %s\n", command);
    }
    //get rid of space in the middle
    while (*command == ' '){
        command++;
    }
    // get arguments
    if (*command == '\0' || *command == '\n'){
        *(arg_buffer) = '\0';
    } else{
        strncpy((int8_t*)arg_buffer, (int8_t*)command, ARG_BUF_SIZE);
    }
    ret = read_dentry_by_name (filename, &tem_dentry);
    if(ret != 0){
        printf("Error: execute_helper, read dentry by name error\n");
        return -1;
    }
    //check file validity
    ret = read_data(tem_dentry.inode_index, 0, (uint8_t*)&magic, 4);
    if(ret == -1){
        printf("Error: read_data error: magic number: %u\n", magic);
        return -1;
    }
    if (magic != MAGIC_EXECUTABLE){
        printf("Error, not executable filename: %s\n", command);
        return -1;
    }
    //printf("IN FUNCTION: EXECUTE_HELPER: MAGIC NUMBER GET!, %u\n", magic);
    ret = read_data(tem_dentry.inode_index, FISRT_INST_ADDR, (uint8_t*)&virtual_addr_1instr, 4);
    if(ret == -1){
        printf("Error: read_data error: first instructino\n");
        return -1;
    }
    if (virtual_addr_1instr == NULL){
        printf("Error, virtual address NULL: %s\n");
        return -1;
    }
    //printf("IN FUNCTION: EXECUTE_HELPER: VIRTUAL ADDR GET!: %x\n", virtual_addr_1instr);
    //find new process number
    for (i = 0; i < MAX_PROCESS_NUMBER; i++){
        if (process_number_array[i] == 0){
            new_process_number = i;
            process_number_array[i] = 1;
            break;
        }
    }
    if (i == MAX_PROCESS_NUMBER){
        printf("No more process are supported.\n");
        return -1;
    }
    // set up paging
    set_paging_directory(KERNEL_END + (new_process_number - 1) * PAGE_SIZE_BIG);
    // load file into memory
    ret = read_data(tem_dentry.inode_index, 0, (uint8_t*)PROGRAM_IMG_VIRT_ADDR, SIZE_PROGRAM_IMG);
    //printf("bytes of file: %d\n", ret);
    new_pcb_ptr = (pcb_t*) (KERNEL_END - KERNEL_STACK_WIDTH * (new_process_number + 1));
    new_pcb_ptr->parent_process_number = cur_process_number;
    new_pcb_ptr->eip_val = virtual_addr_1instr;
    new_pcb_ptr->ebp_val = new_pcb_ptr->esp_val = IMG_BIG_START + PAGE_SIZE_BIG - 4;
    for (i = 0; i < 6; i++){
        new_pcb_ptr->fda[i].fop = NULL;
    }
    // load new arguments
    strncpy((int8_t*)new_pcb_ptr->args, (int8_t*)arg_buffer, ARG_BUF_SIZE);
    pcb_ptr = new_pcb_ptr;
    cur_process_number = new_process_number;
    //printf("IN FUNCTION: EXECUTE_HELPER:\nprocess_number = %d\npcb:%x, %x, %x, %d\n", cur_process_number, pcb_ptr->eip_val, pcb_ptr->esp_val, pcb_ptr->ebp_val, pcb_ptr->parent_process_number);
    tss.esp0 = KERNEL_END - KERNEL_STACK_WIDTH * new_process_number - 4;
    //printf("IN FUNCTION: EXECUTE_HELPER: ANYWAY AT END:\n");
    return (int32_t) pcb_ptr;
}

void set3shells_helper(){
    int ret;
    int i, j;
    dentry_t tem_dentry;
    uint8_t filename[FILENAMESIZE + 1] = "shell";
    uint32_t magic = 0;
    int32_t virtual_addr_1instr;
    int new_process_number;
    pcb_t* new_pcb_ptr;
    ret = read_dentry_by_name (filename, &tem_dentry);
    if(ret != 0){
        printf("Error: set3shells_helper, read dentry by name error\n");
        return;
    }
    //check file validity
    ret = read_data(tem_dentry.inode_index, 0, (uint8_t*)&magic, 4);
    if(ret == -1){
        printf("Error: set3shells_helper: magic number: %u\n", magic);
        return;
    }
    if (magic != MAGIC_EXECUTABLE){
        printf("Error, not executable filename in set3shells.\n");
        return;
    }
    //printf("IN FUNCTION: EXECUTE_HELPER: MAGIC NUMBER GET!, %u\n", magic);
    ret = read_data(tem_dentry.inode_index, FISRT_INST_ADDR, (uint8_t*)&virtual_addr_1instr, 4);
    if(ret == -1){
        printf("Error: read_data error: first instructino\n");
        return;
    }
    if (virtual_addr_1instr == NULL){
        printf("Error, virtual address NULL: %s\n");
        return;
    }
    //printf("IN FUNCTION: EXECUTE_HELPER: VIRTUAL ADDR GET!: %x\n", virtual_addr_1instr);
    //find new process number
    for (i = 3; i >= 1; i--){
        if (process_number_array[i] == 1){
            printf("Error: set3shells_helper: why pid assigned: %d %d?\n", i);
        }
        new_process_number = i;
        process_number_array[i] = 1;
        // set up paging
        set_paging_directory(KERNEL_END + (new_process_number - 1) * PAGE_SIZE_BIG);
        // load file into memory
        ret = read_data(tem_dentry.inode_index, 0, (uint8_t*)PROGRAM_IMG_VIRT_ADDR, SIZE_PROGRAM_IMG);
        //printf("bytes of file: %d\n", ret);
        new_pcb_ptr = (pcb_t*) (KERNEL_END - KERNEL_STACK_WIDTH * (new_process_number + 1));
        new_pcb_ptr->parent_process_number = 0;
        new_pcb_ptr->eip_val = virtual_addr_1instr;
        new_pcb_ptr->ebp_val = new_pcb_ptr->esp_val = IMG_BIG_START + PAGE_SIZE_BIG - 4;
        for (j = 0; j < 6; j++){
            new_pcb_ptr->fda[j].fop = NULL;
        }
        // load new arguments
        strncpy((int8_t*)new_pcb_ptr->args, (int8_t*)"", ARG_BUF_SIZE);
        terminals.terminal[i - 1].pcb_ptr = new_pcb_ptr;
        terminals.terminal[i - 1].pid = new_process_number;
        //printf("IN FUNCTION: EXECUTE_HELPER:\nprocess_number = %d\npcb:%x, %x, %x, %d\n", cur_process_number, pcb_ptr->eip_val, pcb_ptr->esp_val, pcb_ptr->ebp_val, pcb_ptr->parent_process_number);
        //tss.esp0 = KERNEL_END - KERNEL_STACK_WIDTH * new_process_number - 4;
        //printf("IN FUNCTION: EXECUTE_HELPER: ANYWAY AT END:\n");
    }
}

int32_t execute_shell(){
    //printf("OPEN A NEW SHELL.\n");
    return execute((uint8_t*) "shell");
}
/*
 *	Function: halt_helper
 *	Description: helper function of halt system call
 *	inputs:		status -- halt status
 *	outputs:	-1 if fail
 *              256 if exceptions
 *              others with other meaning
 *  Side effects: halt a process and return to father process
 */
int32_t halt_helper(uint8_t status){
    //printf("IN FUNCTION: HALT_HELPER\n");
    int32_t ret = (int32_t) status;
    int i;
    //get old pcb
    int old_process_number = pcb_ptr->parent_process_number;
    pcb_t* old_pcb_ptr = (pcb_t*) (KERNEL_END - KERNEL_STACK_WIDTH * (old_process_number + 1));
    //restore paging
    if (old_process_number > 0){
        set_paging_directory(KERNEL_END + (old_process_number - 1) * PAGE_SIZE_BIG);
    }
    // close fds
    for (i = 2; i < 8; i++){
        pcb_ptr->fda[i - 2].fop = NULL;
    }
    //reset pcb
    process_number_array[cur_process_number] = 0;
    cur_process_number = old_process_number;
    pcb_ptr->eip_val = old_pcb_ptr->eip_val;
    pcb_ptr->esp_val = old_pcb_ptr->esp_val;
    pcb_ptr->ebp_val = old_pcb_ptr->ebp_val;
    pcb_ptr = old_pcb_ptr;
    // reset esp0
    tss.esp0 = KERNEL_END - KERNEL_STACK_WIDTH * old_process_number - 4;
    if (ret == HALT_MAGIC_NUMBER){
        ret = 256;
    }
    //printf("HALT: Back to process_number = %d\n", cur_process_number);
    return ret;
}

/*
 *	Function: open_handler
 *	Description: open system call's handler
 *	inputs:		filename -- the filename to open
 *	outputs:	-1 if fail
 *              fd if success
 *  Side effects: open a file and create a file descripter
 */
int32_t open_handler(const uint8_t* filename){
    //printf("IN FUNCTION: OPEN_HANDLER, open file: %s, len: %d\n", filename, strlen((int8_t*)filename));
    int ret;
    int fd;
    dentry_t tem_dentry;
    ret = read_dentry_by_name (filename, &tem_dentry);
    if(ret != 0){
        printf("cannot open %s, can't find name\n", filename);
        return -1;
    }
    if (tem_dentry.file_type == 2){
        // regular file
        for (fd = 2; fd < 8; fd++){
            if(pcb_ptr->fda[fd-2].fop == NULL){
                pcb_ptr->fda[fd-2].fop = &fop_table_file;
                pcb_ptr->fda[fd-2].inode_idx = tem_dentry.inode_index;
                pcb_ptr->fda[fd-2].file_position = 0;
                pcb_ptr->fda[fd-2].flags = 0;
                return fd;
            }
        }
    } else if (tem_dentry.file_type == 1){
        // directory
        for (fd = 2; fd < 8; fd++){
            if(pcb_ptr->fda[fd-2].fop == NULL){
                pcb_ptr->fda[fd-2].fop = &fop_table_directory;
                pcb_ptr->fda[fd-2].inode_idx = tem_dentry.inode_index;
                pcb_ptr->fda[fd-2].file_position = 0;
                pcb_ptr->fda[fd-2].flags = 0;
                return fd;
            }
        }
    } else if (tem_dentry.file_type == 0){
        // rtc
        for (fd = 2; fd < 8; fd++){
            if(pcb_ptr->fda[fd-2].fop == NULL){
                pcb_ptr->fda[fd-2].fop = &fop_table_rtc;
                //pcb_ptr->fda[fd-2].inode_idx = tem_dentry.inode_index;
                pcb_ptr->fda[fd-2].file_position = 0;
                pcb_ptr->fda[fd-2].flags = 0;
                return fd;
            }
        }
    } else {
        printf("error file type: %s, %u\n", filename, tem_dentry.file_type);
    }
    printf("no availble fd for %s\n", filename);
    return -1;
}

/*
 *	Function: close_handler
 *	Description: close system call's handler
 *	inputs:		fd -- file descriptor index
 *	outputs:	-1 if fail
 *              0 if success
 *  Side effects: close the file descripter
 */
int32_t close_handler(int32_t fd){
    //printf("IN FUNCTION: CLOSE_HELPER: fd = %d\n", fd);
    int ret;
    if (fd < 2 || fd >= 8){
        printf("close_handler: invalid fd: %d\n", fd);
        return -1;
    }
    if(pcb_ptr->fda[fd - 2].fop == NULL){
        printf("close_handler: file not open, fd: %d\n", fd);
        return -1;
    }
    ret = pcb_ptr->fda[fd - 2].fop->close(fd);
    if (ret != 0){
        printf("close_handler: close function error, fd: %d\n", fd);
        return ret;
    }
    pcb_ptr->fda[fd - 2].fop = NULL;
    return 0;
}

/*
 *	Function: read_handler
 *	Description: read system call's handler
 *	inputs:		fd -- file descriptor index
 *              buf -- output buffer
 *              nbytes -- number of bytes to read
 *	outputs:	-1 if fail
 *              number of bytes read
 *  Side effects: put the read value to buf
 */
int32_t read_handler(int32_t fd, void* buf, int32_t nbytes){
    //printf("IN FUNCTION: READ_HELPER\n");
    int ret;
    if (buf == NULL){
        printf("read_handler: NULL buffer, fd: %d\n", fd);
    }
    if (fd == 0){
        // stdin
        return terminal_read(fd, buf, nbytes);
    }
    if (fd < 2 || fd >= 8){
        printf("read_handler: invalid fd: %d\n", fd);
        return -1;
    }
    if(pcb_ptr->fda[fd - 2].fop == NULL){
        printf("read_handler: file not open, fd: %d\n", fd);
        return -1;
    }
    ret = pcb_ptr->fda[fd - 2].fop->read(fd, buf, nbytes);
    if (ret == -1){
        printf("read_handler: read function error, fd: %d\n", fd);
        return ret;
    }
    return ret;
}

/*
 *	Function: write_handler
 *	Description: write system call's handler
 *	inputs:		fd -- file descriptor index
 *              buf -- input buffer
 *              nbytes -- number of bytes to write
 *	outputs:	-1 if fail
 *              0 if success
 *  Side effects: write the buf value to file
 */
int32_t write_handler(int32_t fd, const void* buf, int32_t nbytes){
    //printf("IN FUNCTION: WRITE_HELPER\n");
    int ret;
    if (buf == NULL){
        printf("read_handler: NULL buffer, fd: %d\n", fd);
    }
    if (fd == 1){
        // stdout
        return terminal_write(fd, buf, nbytes);
    }
    if (fd < 2 || fd >= 8){
        printf("write_handler: invalid fd: %d\n", fd);
        return -1;
    }
    if(pcb_ptr->fda[fd - 2].fop == NULL){
        printf("write_handler: file not open, fd: %d\n", fd);
        return -1;
    }
    ret = pcb_ptr->fda[fd - 2].fop->write(fd, buf, nbytes);
    if (ret != 0){
        printf("write_handler: write function error, fd: %d\n", fd);
        return ret;
    }
    return 0;
}

/*
 *	Function: getargs_handler
 *	Description: get arguments for a program
 *	inputs:		buf -- input buffer
 *              nbytes -- number of bytes to get
 *	outputs:	-1 if fail
 *              0 if success
 *  Side effects: write the buf value to file
 */
int32_t getargs_handler(uint8_t* buf, int32_t nbytes){
    //printf("IN FUNCTION: getargs_handler: %s, len: %d\n", pcb_ptr->args, strlen((int8_t*)pcb_ptr->args));
    if (buf == NULL){
        printf("NULL BUFFER.\n");
        return -1;
    }
    if (pcb_ptr->args == NULL){
        printf("NULL ARGUMENTS IN PCB????????.\n");
        return -1;
    }
    if (strlen(pcb_ptr->args) == 0){
        printf("No available arguments\n");
        return -1;
    }
    strncpy((int8_t*)buf, (int8_t*)pcb_ptr->args, nbytes);
    return 0;
}
/*
 *	Function: vidmap_handler
 *	Description: set and get the video page
 *	inputs:		screen_start -- pointer to the output screen start address
 *	outputs:	-1 if fail
 *              0 if success
 *  Side effects: write the *screen_start
 */
int32_t vidmap_handler(uint8_t** screen_start){
    //printf("IN FUNCTION: vidmap_handler\n");
    if (((int)screen_start) - PROGRAM_IMG_VIRT_ADDR < 0 || ((int)screen_start) - PROGRAM_IMG_VIRT_ADDR + 1 >= PAGE_SIZE_BIG){
        printf("INVALID ARGUMENT screen start: %x\n", (uint32_t)screen_start);
        return -1;
    }
    int mem_id;
    mem_id = (terminals.idx_active == terminals.idx_on_screen) ? 0 : (terminals.idx_active + 1);
    set_vedio_paging(VEDIO_PAGES_START, mem_id);
    *screen_start = (uint8_t*) VEDIO_PAGES_START;
    return 0;
}

/* system calls not used, implementing in next time */
int32_t set_handler_handler(int32_t signum, void* handler_address){
    printf("IN FUNCTION: set_handler_handler\n");
    return 0;
}
int32_t sigreturn_handler(void){
    printf("IN FUNCTION: sigreturn_handler\n");
    return 0;
}


