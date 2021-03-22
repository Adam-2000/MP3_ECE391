/*
 *	keyboard.h - Function Header File for keyboard.c
 */
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/* Define necessary parameter*/
#define KEY_NUM         60          // Array of possible character pressed has size 60
#define IRQ_KEYBOARD	1           // Keyboard connected to the IRQ1 of PIC
#define KEY_DATAPORT    0x60        // Keyboard passes in data through this port
#define NULL_KEY	    '\0'        // Some keys are defined to be NULL with value '\0'

/* init the keyboard */
extern void init_keyboard();

/* Keyboard handler */
extern void keyboard_handler();

#endif /* _KEYBOARD_H */
