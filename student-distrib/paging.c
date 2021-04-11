#include "paging.h"
#include "x86_desc.h"
#include "types.h"
#include "lib.h"
#include "paging_enable.h"



paging_dir_entry_t page_directory[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));
paging_table_entry_t page_table[NUM_ENTRY] __attribute__((aligned(ALINED_4K)));

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
    page_directory[0].table_addr = (int)page_table>>PDE_RESERVE_FOR4KB; 

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
        if (i == VEDIO_MEM/ALINED_4K){
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
    //enable page directory by change control reg
    change_control_reg((int)page_directory);
}                                                                                                  

void set_paging_directory(uint32_t phy_addr){
    if (phy_addr == NULL){
        return;
    }
    int i = USER_PAGE_ADDR/ALINED_4K/NUM_ENTRY;
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
    printf("SET PAGING DIRECTORY: %x\n", phy_addr);
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


