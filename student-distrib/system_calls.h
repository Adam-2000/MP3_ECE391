#ifndef SYSTEM_CALLS_H
#define SYSTEM_CALLS_H

#include "types.h"
#include "file_system.h"
#include "system_calls_c.h"
#include "x86_desc.h"
#define MAGIC_EXECUTABLE 0x464c457f
#define SYS_HALT    1
#define SYS_EXECUTE 2
#define SYS_READ    3
#define SYS_WRITE   4
#define SYS_OPEN    5
#define SYS_CLOSE   6
#define SYS_GETARGS 7
#define SYS_VIDMAP  8
#define SYS_SET_HANDLER  9
#define SYS_SIGRETURN  10
//#define ESP_PCB_MASK 0xFFFFE000

#ifndef ASM
//#include "lib.h"
//extern void system_call_func(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3);
// extern void system_call_func();
extern int32_t halt_handler(uint8_t status);
extern int32_t execute_handler(const uint8_t* command);
// extern int32_t read_handler(int32_t fd, void* buf, int32_t nbytes);
// extern int32_t write_handler(int32_t fd, const void* buf, int32_t nbytes);
// extern int32_t open_handler(const uint8_t* filename);
// extern int32_t close_handler(int32_t fd);

/*
 *	Function: system call functions
 *	Description: These functions are system calls that called by user program
 *	inputs:		None
 *	outputs:	None
 *	effects:	Push the arguments to the registers and call int 0x80
 */
extern int32_t halt (uint8_t status);
extern int32_t execute (const uint8_t* command);
extern int32_t read (int32_t fd, void* buf, int32_t nbytes);
extern int32_t write (int32_t fd, const void* buf, int32_t nbytes);
extern int32_t open (const uint8_t* filename);
extern int32_t close (int32_t fd);
extern int32_t getargs (uint8_t* buf, int32_t nbytes);
extern int32_t vidmap (uint8_t** screen_start);
extern int32_t set_handler (int32_t signum, void* handler);
extern int32_t sigreturn (void);

enum signums {
	DIV_ZERO = 0,
	SEGFAULT,
	INTERRUPT,
	ALARM,
	USER1,
	NUM_SIGNALS
};

#endif
#endif
