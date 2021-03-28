
#include "file_system.h"
#include "lib.h"


static inode_t* inode_list;
static boot_block_t* boot_block;
static data_block_t* data_block_list;
static dentry_t* dentry_start;
static file_descriptor_dummy_t fd_dummy;


/*
 * file_system_init
 *   DESCRIPTION: init the pointer point to boot_block, 
 *               and the start block of inode and data_block          
 *   INPUTS: module_start
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void file_system_init(uint32_t module_start){
    cli();
    uint32_t num_inode;
    // initialize the dummy fd
    fd_dummy.inode_idx = -1;
    // init the pointer of boot_block
    boot_block = (boot_block_t*)module_start;
    dentry_start = boot_block->dentry_list;
    //init the start block of inode
    inode_list = (inode_t*)(boot_block + 1);
    //init the start block of datablock
    num_inode = boot_block->inode_num;
    data_block_list = (data_block_t*)(inode_list + num_inode);
    sti();
}

/*
 *  read_dentry_by_name
 *   DESCRIPTION: search the target fname along the  
 *                the boot_block     
 *   INPUTS: fname: the name of target file
 *           dentry: the place we put data from target dentry
 *   OUTPUTS: none
 *   RETURN VALUE: 0 as success; other as false
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    int i;
    uint32_t fname_len = strlen((int8_t*)fname);
    if (fname_len > NAME_LEN - 1){
        return -1;
    }

    // search along the dentry in boot_block
    uint32_t tem_len;
    for (i = 0; i < boot_block->dentry_num; i++){
        //check whether they are the same
        if (fname_len == tem_len){
            if (!strncmp((int8_t*)fname, (int8_t*)dentry_start[i].file_name, NAME_LEN)){
                return read_dentry_by_index(i, dentry);
            }
        }
    }
    return -1;
}


/*
 *  read_dentry_by_index
 *   DESCRIPTION: after we find target file_name, we copy the information  
 *                from block_list into dentry we push in
 *   INPUTS: index: the index of the nodes
 *           dentry: the place we put data from target dentry
 *   OUTPUTS: none
 *   RETURN VALUE: 0 as success; other as false
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    // check for valid inode #
    if (index >= boot_block->inode_num){
        return -1;
    }
    // Copy the information from dentry
    strncpy((int8_t*)dentry->file_name, (int8_t*)dentry_start[index].file_name,NAME_LEN);
    dentry->file_type = dentry_start[index].file_type;
    fd_dummy.inode_idx = dentry->inode_index = dentry_start[index].inode_index;

    return 0;
}


/*
 *  read_data
 *   DESCRIPTION: read data from the given file information 
 *   INPUTS: inode: the index of the nodes
 *           offset: the offset of bytes of a file we start to read
 *           buf: the buffer where to store data
 *           length
 *   OUTPUTS: none
 *   RETURN VALUE: 0 as reaching the end of the file
 *                 other as the num of the bytes we read
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){

    inode_t* inode_curr = (inode_t*)(inode_list+inode);
    uint32_t file_length = inode_curr->f_length;           
    uint32_t data_block_index = offset / BLOCK_SIZE;     
    uint32_t data_index = offset % BLOCK_SIZE;           
    uint32_t byte_num = 0;
    uint32_t data_curr_index;
    
    if (buf == NULL){
        return -1;
    }

    int i;
    for(i = 0 ; i<length; i++, data_index++){
        if (data_index >= BLOCK_SIZE){
            data_index = 0;
            data_block_index++;
        }
        if (i + offset >= file_length){
            break;
        }
        data_curr_index = inode_curr->data_block_list[data_block_index];
        if(data_curr_index >= boot_block->data_block_num){
            return -1;
        }
        buf[i] = data_block_list[data_curr_index].data_block_content[data_index];
        byte_num++;
    }
    return byte_num;
}



int32_t file_read(int32_t fd, void* buf, int32_t nbytes){  
    if (buf == NULL || nbytes < 0 || fd_dummy.inode_idx <= 0){
        return -1;
    }
    fd_dummy.offset_rec += read_data(fd_dummy.inode_idx, fd_dummy.offset_rec, (uint8_t*)buf, nbytes);
    return 0;
}

int32_t file_open(const uint8_t* filename){
    int ret;
    dentry_t* tem_dentry;
    ret = read_dentry_by_name (filename, tem_dentry);
    if(ret != 0){
        return -1;
    }
    fd_dummy.offset_rec = 0;
    return 0;
}

int32_t file_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}

int32_t file_close(int32_t fd){
    fd_dummy.inode_idx = -1;
    return 0;
}

int32_t directory_open(const uint8_t* filename) {
    fd_dummy.inode_idx = 0;
    return 0;
}

int32_t directory_close(int32_t fd) {
    fd_dummy.inode_idx = -1;
    return 0;
}

int32_t directory_read(int32_t fd, void* buf, int32_t nbytes) {
    int i;
    uint8_t* buffer;
    // only read one file name at a time
    if (fd_dummy.inode_idx != 0 || nbytes != NAME_LEN || fd_dummy.offset_rec >= boot_block->dentry_num){
        return -1;
    }
    buffer = (uint8_t*) buf;
    for(i = 0; i < NAME_LEN; i++){
        buffer[i] = dentry_start[fd_dummy.offset_rec].file_name[i];
    }
    fd_dummy.offset_rec++;
    return 0;

}

int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}










