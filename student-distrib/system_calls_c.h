#ifndef SYSTEM_CALLS_C_H
#define SYSTEM_CALLS_C_H

#include "types.h"
#include "file_system.h"
#include "rtc.h"
#include "keyboard.h"
#include "paging.h"
#include "x86_desc.h"
#define MAX_PROCESS_NUMBER 7
#define KERNEL_END 0x800000     //8MB
#define KERNEL_STACK_WIDTH 0x2000   //8KB
#define SIZE_PROGRAM_IMG 0x400000 - 0x48000 //4MB - something
#define PAGE_SIZE_BIG 0x400000 //4MB
#define PAGE_SIZE_SMALL 0x1000 //4kB
#define IMG_BIG_START 0x8000000 //128MB
#define PROGRAM_IMG_VIRT_ADDR 0x08048000
#define HALT_MAGIC_NUMBER 0x98
#define FILENAMESIZE 32
#define FISRT_INST_ADDR 24
#define VEDIO_PAGES_START (IMG_BIG_START + PAGE_SIZE_BIG)

#ifndef ASM
//#include "lib.h"
//extern void system_call_func(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3);
// extern void system_call_func();
// extern int32_t halt_handler(uint8_t status);
// extern int32_t execute_handler(const uint8_t* command);
// current process number
extern int cur_process_number;
/*read system call's handler*/
extern int32_t read_handler(int32_t fd, void* buf, int32_t nbytes);
/*write system call's handler*/
extern int32_t write_handler(int32_t fd, const void* buf, int32_t nbytes);
/*open system call's handler*/
extern int32_t open_handler(const uint8_t* filename);
/*close system call's handler*/
extern int32_t close_handler(int32_t fd);

/*execute system call's handler's helper function*/
extern int32_t execute_helper(const uint8_t* command);
/*halt system call's handler's helper function*/
extern int32_t halt_helper(uint8_t status);

/* getargs syscall handler*/
extern int32_t getargs_handler(uint8_t* buf, int32_t nbytes);
/* vidmap syscall handler*/
extern int32_t vidmap_handler(uint8_t** screen_start);

/* system call not used, implementing in next time */
extern int32_t set_handler_handler(int32_t signum, void* handler_address);
extern int32_t sigreturn_handler(void);

/* initialize system calls*/
extern void system_calls_init();
/* get the pcb pointer*/
extern uint32_t get_pcb_ptr();
extern void set_pcb_ptr(pcb_t* new_pcb_ptr);
extern int32_t execute_shell();

extern void set3shells_helper();
#endif
#endif
