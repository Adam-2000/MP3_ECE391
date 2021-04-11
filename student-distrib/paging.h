#ifndef _PAGING_H
#define _PAGING_H


#include "x86_desc.h"
#include "types.h"
#include "lib.h"
#include "paging_enable.h"

#define NUM_ENTRY 1024 //(2^10)
#define ALINED_4K 4096 //(2^12)
#define START_KERNAL_ADDR 0X400000
#define VEDIO_MEM 0xB8000
#define PDE_RESERVE_FOR4KB 12   //num of bits reserved
#define USER_PAGE_ADDR 0X8000000 //128 MB the user stack address in virtual mem

#ifndef ASM

// struct for both two types of pde
typedef struct __attribute__((packed)) paging_dir_entry{
    uint32_t present        :1;     //1 valid, 0 not exist
    uint32_t r_w            :1;
    uint32_t u_s            :1;
    uint32_t pwt            :1;     //always be zero
    uint32_t pcd            :1;
    uint32_t access         :1;
    uint32_t reserved       :1;     //always set to one
    uint32_t page_size      :1;     //0 for 4k,1 for 4M
    uint32_t global_bit     :1;     //ignored for 4K page directory entries
    uint32_t avail          :3;
    uint32_t table_addr      :20;
}paging_dir_entry_t;

// struct for pte
typedef struct __attribute__((packed)) paging_table_entry{
    uint32_t present        :1;     //1 valid, 0 not exist
    uint32_t r_w            :1;
    uint32_t u_s            :1;
    uint32_t pwt            :1;
    uint32_t pcd            :1;
    uint32_t access         :1;
    uint32_t dirty          :1;
    uint32_t pat_alw_zero   :1;     // this bit is always 0
    uint32_t global_bit     :1;
    uint32_t avail          :3;
    uint32_t page_addr     :20;
}paging_table_entry_t;

extern paging_dir_entry_t page_directory[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));

extern paging_table_entry_t page_table[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));

extern void paging_init(void);

extern void set_paging_directory(uint32_t phy_addr);
#endif
#endif
