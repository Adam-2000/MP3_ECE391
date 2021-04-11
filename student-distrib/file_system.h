#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include "types.h"
#include "system_calls_c.h"

#define DIR_NUM 63
#define NAME_LEN 32
#define DATA_BLOCK_OF_INODE 1024-1
#define BLOCK_SIZE 4096
#define BOOT_RESERVED_NUM 52
#define DEN_RESERVED_NUM 24
#ifndef ASM

/*dentry stucture*/
typedef struct dentry {
    uint8_t file_name[NAME_LEN];
    uint32_t file_type;
    uint32_t inode_index;
    uint8_t reserved[DEN_RESERVED_NUM];
} dentry_t;

/*boot_block stucture*/
typedef struct boot_block {
    uint32_t dentry_num;
    uint32_t inode_num;
    uint32_t data_block_num;
    uint8_t  reserved[BOOT_RESERVED_NUM];
    dentry_t dentry_list[DIR_NUM];
} boot_block_t;

/*inode stucture*/
typedef struct inode {
    uint32_t f_length;
    uint32_t data_block_list[DATA_BLOCK_OF_INODE];
} inode_t;

/*data_block stucture*/
typedef struct data_block{
    uint8_t data_block_content[BLOCK_SIZE];
}data_block_t;

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

typedef struct pcb{
    uint32_t eip_val;
    uint32_t esp_val;
    uint32_t ebp_val;
    int parent_process_number;
    //pcb_t* parent_pcb_ptr;
    //uint32_t physical_addr; 
    file_descriptor_t fda[6];
}pcb_t;

/*initialize file system*/
void file_system_init(uint32_t module_start);
/*read dentry by name*/
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
/*read dentry by index*/
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
/*read data to the buffer*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/*read data to the buffer*/
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
/*open a file*/
int32_t file_open(const uint8_t* filename);
/*write to a file*/
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
/*close a file*/
int32_t file_close(int32_t fd);

/*open a directory*/
int32_t directory_open(const uint8_t* filename);
/*close a directory*/
int32_t directory_close(int32_t fd);
/*read data to the buffer*/
int32_t directory_read(int32_t fd, void* buf, int32_t nbytes);
/*write to a directory*/
int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes);

#endif
#endif
