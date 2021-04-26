/*
 *	keyboard.h - Function Header File for keyboard.c
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "i8259.h"
#include "x86_desc.h"
#include "system_calls_c.h"
#include "file_system.h"
#include "paging.h"
/* Define necessary parameter*/
#define KEY_NUM         59          // Array of possible character pressed has size 60
#define IRQ_KEYBOARD	1           // Keyboard connected to the IRQ1 of PIC
#define KEY_DATAPORT    0x60        // Keyboard passes in data through this port
#define NULL_KEY	    '\0'        // Some keys are defined to be NULL with value '\0'

#define SCREEN_WIDTH    80

#ifndef ASM
#include "lib.h"

extern terminals_info_t terminals;
/* init the keyboard */
extern void init_keyboard();

/* Keyboard handlers */
extern void keyboard_handler();

extern int32_t terminal_open(const uint8_t* filename);

extern int32_t terminal_read(int32_t fd, void* buffer, int32_t nbytes);

extern int32_t terminal_write(int32_t fd, const void* buffer, int32_t nbytes);

extern int32_t terminal_close(int32_t fd);
#endif /* ASM */

#endif /* _KEYBOARD_H */
