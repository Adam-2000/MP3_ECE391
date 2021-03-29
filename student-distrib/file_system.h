#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include "types.h"

#define DIR_NUM 63
#define NAME_LEN 32
#define DATA_BLOCK_OF_INODE 1024-1
#define BLOCK_SIZE 4096
#define BOOT_RESERVED_NUM 52
#define DEN_RESERVED_NUM 24
#ifndef ASM

typedef struct dentry {
    uint8_t file_name[NAME_LEN];
    uint32_t file_type;
    uint32_t inode_index;
    uint8_t reserved[DEN_RESERVED_NUM];
} dentry_t;


typedef struct boot_block {
    uint32_t dentry_num;
    uint32_t inode_num;
    uint32_t data_block_num;
    uint8_t  reserved[BOOT_RESERVED_NUM];
    dentry_t dentry_list[DIR_NUM];
} boot_block_t;


typedef struct inode {
    uint32_t f_length;
    uint32_t data_block_list[DATA_BLOCK_OF_INODE];
} inode_t;

typedef struct data_block{
    uint8_t data_block_content[BLOCK_SIZE];
}data_block_t;

typedef struct file_descriptor_dummy{
    int32_t offset_rec;
    int32_t inode_idx;
}file_descriptor_dummy_t;

void file_system_init(uint32_t module_start);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);


int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
int32_t file_open(const uint8_t* filename);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t file_close(int32_t fd);

int32_t directory_open(const uint8_t* filename);
int32_t directory_close(int32_t fd);
int32_t directory_read(int32_t fd, void* buf, int32_t nbytes);
int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes);

#endif
#endif
