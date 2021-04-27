#ifndef _PIT_H
#define _PIT_H

#include "types.h"
#include "i8259.h"
#include "system_calls_c.h"

#define PIT_DATA_PORT0 0x40
#define PIT_CMD_PORT 0x43
#define PIT_MODE_SET 0x36 // channel 0, access mode 3, operating mode 3, binary mode
#define PIT_FREQ_SET 29830 //1193182 Hz / 40. with mode 3, actually frequency is 20Hz


#ifndef ASM

extern void pit_init();
extern int32_t PIT_handler_helper();


#endif /*ASM*/
#endif /*_PIT_H*/
