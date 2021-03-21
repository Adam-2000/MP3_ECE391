/*
*	keyboard.c - handles interrupts received from keyboard
*/

#include "keyboard.h"
#include "lib.h"
#include "types.h"
#include "i8259.h"
#include "x86_desc.h"

unsigned char pressed_key = 0;              // char to receive the keyboard input data
unsigned char echo = 0;                     // key echo to display on screen
static unsigned char keymap[KEY_NUM] =      // possible key contents
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
    while(1){
        // Check whether the input value is in range for reasonable key
        if((inb(KEY_DATAPORT) > 0) && (inb(KEY_DATAPORT) < KEY_NUM)){
            pressed_key = inb(KEY_DATAPORT);
            break;
        }
    }

    echo = keymap[pressed_key];
    // Handle NULL key
    if(echo == NULL_KEY){
        return;
    } 
    
    putc(echo);

    send_eoi(IRQ_KEYBOARD);
    sti();
}
