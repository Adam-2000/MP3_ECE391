#include "paging.h"
#include "types.h"
#include "lib.h"

void paging_init(void){

    //set the first 4MB as 4kB pages
    page_directory[0].present = 1;
    page_directory[0].r_w = 1;
    page_directory[0].u_s = 0; 
    page_directory[0].pwt = 0;
    page_directory[0].pcd = 0; 
    page_directory[0].access = 0;
    page_directory[0].reserved = 0;
    page_directory[0].page_size = 0; // set size to 0 as 4KB pages
    page_directory[0].table_addr = (int)page_table;

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

    int i;
    // set the following pde as not present 
    for (i=2;i<NUM_ENTRY;i++){
        page_directory[i].present = 0;
    }

    // init the page table for 0-4MB address
    for (i=0;i<NUM_ENTRY;i++){
        if (i == VEDIO_MEM/ALINED_4K){
            // set the target vedio memory page as present 
            page_table[i].present =  1;
            page_table[i].read_write    = 1;
            page_table[i].user          = 0;
            page_table[i].write_through = 0;  
            page_table[i].cache_disable = 1;  
            page_table[i].accessed      = 0;
            page_table[i].dirty         = 0;
            page_table[i].reserved      = 0;
            page_table[i].global        = 0;  
            page_table[i].page_addr_31_12 = i;
        }
        else {
            // set other page as not present
            page_table[i].present = 0;
        }
    }
}

