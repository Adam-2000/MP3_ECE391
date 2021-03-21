#include "x86_desc.h"
#include "types.h"
#include "lib.h"



#define NUM_ENTRY 2^10
#define ALINED_4K 2^12
#define START_KERNAL_ADDR 0X400000
#define VEDIO_MEM 0xB8000
#define PDE_RESERVE_FOR4KB 12
#define PDE_RESERVE_FOR4MB 22


typedef struct _attribute_((packed)){
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
    uint8_t table_addr      :20;
}paging_dir_entry_t;

typedef struct _attribute_((packed)){
    uint32_t present        :1;     //1 valid, 0 not exist
    uint32_t r_w            :1;
    uint32_t u_s            :1;
    uint32_t pwt            :1;
    uint32_t pct            :1;
    uint32_t access         :1;
    uint32_t dirty          :1;
    uint32_t pat_alw_zero   :1;
    uint32_t global_bit     :1;
    uint32_t avail          :3;
    uint8_t page_addr     :20;
}paging_table_entry_t;

dir_entry_desc_t page_directory[NUM_ENTRY] _attribute_(aligned(ALINED_4K));

paging_dir_entry_t page_table[NUM_ENTRY] _attribute_(aligned(ALINED_4K));



