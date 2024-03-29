/* types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0
#define ARG_BUF_SIZE 1024
#define SIZE_KEYBOARD_BUFFER 128
#define N_TERMINAL 3

#ifndef ASM

/* Types defined here just like in <stdint.h> */
typedef int int32_t;
typedef unsigned int uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

/*function operations table*/
typedef struct fop_table{
    int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*open)(const uint8_t* filename);
    int32_t (*close) (int32_t fd);
}fop_table_t;

/*fd stucture*/
typedef struct file_descriptor{
    fop_table_t* fop;
    uint32_t inode_idx;
    uint32_t file_position;
    uint32_t flags;
}file_descriptor_t;

/*pcb struct*/
typedef struct pcb{
    uint32_t eip_val;
    uint32_t esp_val;
    uint32_t ebp_val;
    int parent_process_number;
    file_descriptor_t fda[6];
    char args[ARG_BUF_SIZE];
}pcb_t;


/*Basic keyboard buffer structure*/
typedef struct keyboard_buffer_struct{
    char buffer[SIZE_KEYBOARD_BUFFER];
    uint32_t cnt;       //counter
    uint32_t enable;
    //uint32_t cursor; // this saves for cursor operation // don't delete
} keyboard_buffer_struct_t;

/*terminal struct for each terminal*/
typedef struct terminal_info{
    keyboard_buffer_struct_t key_buffer;
    uint32_t screen_x;
    uint32_t screen_y;
    pcb_t* pcb_ptr;
    int pid;
} terminal_info_t;

/* a struct, globle, terminals information*/
typedef struct terminals_info{
    int idx_on_screen;   // the showing terminal index
    int idx_active;      // the back running terminal index
    terminal_info_t terminal[N_TERMINAL];
} terminals_info_t;
#endif /* ASM */

#endif /* _TYPES_H */
