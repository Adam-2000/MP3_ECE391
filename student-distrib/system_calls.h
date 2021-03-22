#ifndef SYSTEM_CALL_C
#define SYSTEM_CALL_C

#include "types.h"
#ifndef ASM
#include "lib.h"
//extern void system_call_func(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3);
extern void system_call_func();
#endif
#endif
