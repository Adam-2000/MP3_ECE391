/*
*	keyboard.c - handles interrupts received from keyboard
*/

#include "keyboard.h"

static char keymap[KEY_NUM] =             // possible key contents
	{'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
	 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's',
	 'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 
	 'b', 'n', 'm',',', '.', '/', '\0', '*', '\0', ' ', '\0'};

static char keyboard_buffer[SIZE_KEYBOARD_BUFFER];
static uint32_t key_buf_cnt;
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
    uint32_t char_index;
    char key_char;

    cli();

    // receive the scancode through keyboard's dataport
    char_index = inb(KEY_DATAPORT);
    if(char_index < KEY_NUM){
        // echo the corresponding key from the array
        key_char = keymap[char_index];
        // check for NULL key
        if(key_char != '\0'){
            putc(key_char);
        }
    }

    send_eoi(IRQ_KEYBOARD);
    sti();
}

int32_t keyboard_open(){
    enable_irq(IRQ_KEYBOARD);
    return 0;
}

int32_t keyboard_read(char* buffer);

int32_t keyboard_write(const char* buffer);

int32_t keyboard_close(){
    disable_irq(IRQ_KEYBOARD);
    return 0;
}
