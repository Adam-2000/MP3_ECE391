/*
*	keyboard.c - handles interrupts received from keyboard
*/

#include "keyboard.h"

static uint8_t keymap[KEY_NUM] =      // possible key contents
	{'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
	 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's',
	 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 
	 'b', 'n', 'm',',', '.', '/', '\0', '*', '\0', ' ', '\0'};

/*
  *	Function: init_keyboard
 *	Description: This function initializes the keyboard to IRQ 1 on the PIC
 *	inputs:		None
 *	outputs:	None
 *	effects:	Motivate IRQ 1 on the master PIC
 */ 
void init_keyboard() {
	enable_irq(IRQ_KEYBOARD);
}

/*
 *	Function: keyboard_handler
 *	Description: This function handles the interrupts from keyboard, read the 
 *                input data, then echo and display the corresponding key value
 *	inputs:		None
 *	outputs:	None
 *	effects:	Handle the keyboard interrup and display corresponding key on screen
 */
void keyboard_handler(){
    cli();
    printf("keyboard yeah\n");
    uint32_t key_idx = inb(KEY_DATAPORT);
    uint8_t key_char;

    key_idx = inb(KEY_DATAPORT);
    if (key_idx < KEY_NUM){
        key_char = keymap[key_idx];
        printf("keyboard yeah: %c\n", key_char);
        if(key_char != '\0'){
            putc(key_char);
        }
    }
    send_eoi(IRQ_KEYBOARD);
    sti();
}
