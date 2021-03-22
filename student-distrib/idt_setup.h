#ifndef _IDT_SETUP_H
#define _IDT_SETUP_H

#define NUM_EXCEPTION 0x14
#define INTERRPUT_START_INDEX 0x20
#define INTERRPUT_END_INDEX 0x2F
#define KEYBOARD_INT_INDEX 0x21
#define RTC_INT_INDEX 0x28
#define SYS_CALL_INDEX 0x80

#include "x86_desc.h"
#include "exceptions.h"
#include "system_calls.h"
#include "interrupts.h"
#include "idt_handler_preset.h"

#ifndef ASM
void idt_init();
#endif


#endif
