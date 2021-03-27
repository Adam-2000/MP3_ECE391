/*
 *	keyboard.h - Function Header File for keyboard.c
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"
#include "i8259.h"
#include "x86_desc.h"
/* Define necessary parameter*/
#define KEY_NUM         59          // Array of possible character pressed has size 60
#define IRQ_KEYBOARD	1           // Keyboard connected to the IRQ1 of PIC
#define KEY_DATAPORT    0x60        // Keyboard passes in data through this port
#define NULL_KEY	    '\0'        // Some keys are defined to be NULL with value '\0'

#define SIZE_KEYBOARD_BUFFER 128
#define SCREEN_WIDTH    80

#ifndef ASM
#include "lib.h"
/* init the keyboard */
extern void init_keyboard();

/* Keyboard handler */
extern void keyboard_handler();

extern int32_t terminal_open();

extern int32_t terminal_read(int32_t fd, char* buffer, int32_t nbytes);

extern int32_t terminal_write(int32_t fd, const char* buffer, int32_t nbytes);

extern int32_t terminal_close();
#endif /* ASM */

#endif /* _KEYBOARD_H */
