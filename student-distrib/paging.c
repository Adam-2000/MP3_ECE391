#include "paging.h"
#include "x86_desc.h"
#include "types.h"
#include "lib.h"
#include "paging_enable.h"



paging_dir_entry_t page_directory[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));
paging_table_entry_t page_table[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));
paging_table_entry_t page_table_video[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));

/*
 * paging_init
 *   DESCRIPTION: init the page directory entry and page 
 *                table entry for paging
 *   INPUTS: none
 *   OUTPUTS: page_directory
 *   RETURN VALUE: none
 */
void paging_init(){
    int i;  //index
    //set the first 4MB as 4kB pages
    cli();
    page_directory[0].present = 1;
    page_directory[0].r_w = 1;
    page_directory[0].u_s = 0; 
    page_directory[0].pwt = 0;
    page_directory[0].pcd = 0; 
    page_directory[0].access = 0;
    page_directory[0].reserved = 0;
    page_directory[0].page_size = 0; // set size to 0 as 4KB pages
    // because when we use 20 bit table base address, we would add 12 zero at right
    // thus we need to shift to right 12 bit
    page_directory[0].table_addr = (uint32_t)page_table>>PDE_RESERVE_FOR4KB; 

    //set the 4MB-8MB as one 4MB page
    page_directory[1].present = 1;
    page_directory[1].r_w = 1;
    page_directory[1].u_s = 0; 
    page_directory[1].pwt = 0;
    page_directory[1].pcd = 0; 
    page_directory[1].access = 0;
    page_directory[1].reserved = 0; // this is D for 4MB pde
    page_directory[1].page_size = 1; // set size to 1 as 4MB pages

    //because if the page is 4MB, bit 21-12 of pde neet to be zero
    //thus bit 31-12 would be 0000 0000 0100 0000 0000
    page_directory[1].table_addr = (START_KERNAL_ADDR/ALINED_4K/NUM_ENTRY)<<10;

    // set the following pde as not present 
    for (i=2;i<NUM_ENTRY;i++){
        page_directory[i].present = 0;
    }

    // init the page table for 0-4MB address
    for (i=0;i<NUM_ENTRY;i++){
        page_table[i].present = 0;
        if (i >= VEDIO_MEM/ALINED_4K || i < VEDIO_MEM/ALINED_4K + 3){
            // set the target vedio memory page as present 
            page_table[i].present        = 1;
            page_table[i].r_w            = 1;
            page_table[i].u_s            = 0;
            page_table[i].pwt            = 0;  
            page_table[i].pcd            = 1;  
            page_table[i].access         = 0;
            page_table[i].dirty          = 0;
            page_table[i].pat_alw_zero   = 0;
            page_table[i].global_bit     = 0; 

            // the page base address is the index (total 2^10)
            page_table[i].page_addr      = i; 
        }
    }
    // set video page directory
    i = VEDIO_PAGES_START/ALINED_4K/NUM_ENTRY;
    page_directory[i].present = 1;
    page_directory[i].r_w = 1;
    page_directory[i].u_s = 1; 
    page_directory[i].pwt = 0;
    page_directory[i].pcd = 0; 
    page_directory[i].access = 0;
    page_directory[i].reserved = 0; 
    page_directory[i].page_size = 0;
    page_directory[i].table_addr = (uint32_t)page_table_video>>PDE_RESERVE_FOR4KB; 
    // init the video page table for 128MB + (3 ~ 4) * 4MB 
    for (i=0;i<NUM_ENTRY;i++){
        page_table_video[i].present = 0;
    }
    //enable page directory by change control reg
    change_control_reg((int)page_directory);
    sti();
}                                                                                                  

/*
 * set_paging_directory
 *   DESCRIPTION: set a page directory for new physical address
 *   INPUTS: phy_addr -- the physical address of the new page for program img
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void set_paging_directory(uint32_t phy_addr){
    if (phy_addr == NULL){
        return;
    }
    uint32_t flag;
    int i = USER_PAGE_ADDR/ALINED_4K/NUM_ENTRY;
    cli_and_save(flag);
    page_directory[i].present = 1;
    page_directory[i].r_w = 1;
    page_directory[i].u_s = 1; 
    page_directory[i].pwt = 0;
    page_directory[i].pcd = 0; 
    page_directory[i].access = 0;
    page_directory[i].reserved = 0; 
    page_directory[i].page_size = 1;
    page_directory[i].table_addr = (phy_addr/ALINED_4K/NUM_ENTRY)<<10;

    reload_cr3((int)page_directory);
    restore_flags(flag);
    //printf("SET PAGING DIRECTORY: %x\n", phy_addr);
}

/*
 * set_vedio_paging
 *   DESCRIPTION: set a virtual address to the video page
 *   INPUTS: virtual_addr -- the virtual address of the new page for video 
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void set_vedio_paging(uint32_t virtual_addr, int mem_id){
    int idx;
    if (virtual_addr == NULL || (virtual_addr & DIRECTORY_MASK) != VEDIO_PAGES_START){
        printf("wrong virtual address when set paging: %x\n", virtual_addr);
        return;
    }
    uint32_t flags;
    cli_and_save(flags);
    idx = virtual_addr & TABLE_MASK >> PDE_RESERVE_FOR4KB;
    // set the target vedio memory page as present 
    page_table_video[idx].present        = 1;
    page_table_video[idx].r_w            = 1;
    page_table_video[idx].u_s            = 1;
    page_table_video[idx].pwt            = 0;  
    page_table_video[idx].pcd            = 1;  
    page_table_video[idx].access         = 0;
    page_table_video[idx].dirty          = 0;
    page_table_video[idx].pat_alw_zero   = 0;
    page_table_video[idx].global_bit     = 0; 
    page_table_video[idx].page_addr      = (VEDIO_MEM + PAGE_SIZE_SMALL * mem_id) >> PDE_RESERVE_FOR4KB; 
    reload_cr3((int)page_directory);
    restore_flags(flags);
    //printf("SET VEDIO PAGE TABLE: %x\n", virtual_addr);
}

/*
 * set_vedio_pageoff
 *   DESCRIPTION: turn off a page
 *   INPUTS: virtual_addr -- the virtual address of the new page for video 
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void set_vedio_pageoff(uint32_t virtual_addr){
    int idx;
    if (virtual_addr == NULL || (virtual_addr & DIRECTORY_MASK) != VEDIO_PAGES_START){
        printf("wrong virtual address when set paging: %x\n", virtual_addr);
        return;
    }
    uint32_t flags;
    cli_and_save(flags);
    idx = virtual_addr & TABLE_MASK >> PDE_RESERVE_FOR4KB;
    // set the target vedio memory page as present 
    page_table_video[idx].present        = 0;
    reload_cr3((int)page_directory);
    restore_flags(flags);
    //printf("SET VEDIO PAGE TABLE: %x\n", virtual_addr);
}
/*
    asm volatile("                                          \n\
        movl %0, %%eax                                      \n\
        movl %%eax, %%cr3                                   \n\
        movl %%cr4, %%eax                                   \n\
        orl $0x10, %%eax                                    \n\
        movl %%eax, %%cr4                                   \n\
        movl %%cr0, %%eax                                   \n\
        orl $0x80000000, %%eax                              \n\
        movl %%eax, %%cr0                                   \n\
        "
        :
        : "r" (page_dir)
        : "eax"
    );

*/


