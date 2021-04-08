#ifndef SYSTEM_CALL_C
#define SYSTEM_CALL_C

#include "types.h"
#ifndef ASM
#include "lib.h"
//extern void system_call_func(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3);
extern void system_call_func();
// extern int32_t halt(uint8_t status);
// extern int32_t execute(const uint8_t* command);
// extern int32_t read(int32_t fd, void* buf, int32_t nbytes);
// extern int32_t write(int32_t fd, const void* buf, int32_t nbytes);
// extern int32_t open(const uint8_t* filename);
// extern int32_t close(int32_t fd);
#endif
#endif
