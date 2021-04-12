/*
*	keyboard.c - handles interrupts received from keyboard
*/

#include "keyboard.h"

/*Scan codes*/
#define RELEASE_SIGN 0x80
#define CAPITAL_OFF 'a' - 'A'

#define BACKSPACE 0x0E
#define TAB 0x0F
#define ENTER 0x1C
#define CTRL 0x1D
#define SHIFT 0x2A
#define SHIFT_R 0x36
#define ALT 0x38
#define CAPSLOCK 0x3A

#define KEYBOARD_SPECTIAL 0xE0
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4B
#define RIGHT 0x4D
#define ALT_R 0x11
#define CTRL_R 0x1D

/*Normal key pad*/
static char keymap[KEY_NUM] = {
    '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 
	'b', 'n', 'm',',', '.', '/', '\0', '\0', '\0', ' ', '\0'
};

/*Special key pad with SHIFT or CTRL*/
static char shifted_keymap[KEY_NUM] = {
    '\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\0', '\0', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L' , ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 
	'B', 'N', 'M','<', '>', '?', '\0', '\0', '\0', ' ', '\0'
};

/*Basic keyboard buffer structure*/
static struct keyboard_buffer_struct_t{
    char buffer[SIZE_KEYBOARD_BUFFER];
    uint32_t cnt;       //counter
    uint32_t enable;
    //uint32_t cursor; // this saves for cursor operation // don't delete
} key_buffer;

/*Basic keyboard status structure*/
struct key_status_struct{
    uint32_t shift : 1;
    uint32_t shift_r : 1;
    uint32_t alt : 1;
    uint32_t alt_r : 1;
    uint32_t ctrl : 1;
    uint32_t ctrl_r : 1;
    uint32_t capital_status : 1;
    uint32_t padding : 25;
};
//} __attribute__ ((packed)) key_status;

static union key_status_t {
    uint32_t val;
    struct key_status_struct key_status;
} key_state;

#define key_status key_state.key_status
/*
  *	Function: init_keyboard
 *	Description: This function initializes the keyboard to IRQ 1 on the PIC
 *	inputs:		None
 *	outputs:	None
 *	effects:	Motivate IRQ 1 on the master PIC
 */ 
void init_keyboard() {
    key_state.val = 0;
    key_buffer.cnt = 0;
    key_buffer.enable = 0;
    enable_cursor(14, 15);
    //key_buffer.cursor = 0;
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
    uint32_t char_index;    // receive the scan code
    char key_char;          // corresponding key value
    int i;
    cli();

    // receive the scancode through keyboard's dataport
    char_index = inb(KEY_DATAPORT);
    // receive 0xE0, then read for next byte. useful for directions and right ctrl, right alt
    if (char_index == KEYBOARD_SPECTIAL){
        char_index = inb(KEY_DATAPORT);
        // released button updates status
        if (char_index & RELEASE_SIGN){
            char_index &= ~RELEASE_SIGN;
            switch (char_index) {
                case ALT_R:
                    key_status.alt_r = 0;
                    break;
                case CTRL_R:
                    key_status.ctrl_r = 0;
                    break;
                default:
                    break;
            }
            goto END;
        }
        switch (char_index) {
            case ALT_R:
                key_status.alt_r = 1;
                break;
            case CTRL_R:
                key_status.ctrl_r = 1;
                break;
            // cursor control and history input lines. save for next time
            case UP:
                break;
            case DOWN:
                break;
            case LEFT:
                break;
            case RIGHT:
                break;
            default:
                break;
        }
        goto END;
    }
    // released button updates status
    if (char_index & RELEASE_SIGN){
        char_index &= ~RELEASE_SIGN;
        switch (char_index) {
            case SHIFT:
                key_status.shift = 0;
                break;
            case SHIFT_R:
                key_status.shift_r = 0;
                break;
            case ALT:
                key_status.alt = 0;
                break;
            case CTRL:
                key_status.ctrl = 0;
                break;
            default:
                break;
        }
        goto END;
    }
    // check whether in range
    if(char_index < KEY_NUM){
        key_char = keymap[char_index];
        // check for NULL value
        if(key_char != '\0'){
            if (key_char >= 'a' && key_char <= 'z'){
                // check for  ctrl + l, which means clearing the screen
                if (key_char == 'l' && (key_status.ctrl | key_status.ctrl_r)){
                    //key_buffer.cnt = 0;
                    clear();
                    set_cursor(0, 0);
                    goto END;
                }
                // capitalize characters
                if ((key_status.shift | key_status.shift_r) ^ key_status.capital_status){
                    key_char -= CAPITAL_OFF;
                }
            } else if(key_status.shift | key_status.shift_r){
                // shift key for non-character inputs
                key_char = shifted_keymap[char_index];
            }
            // if the buffer has only 1 space left, then don't put the new character. 
            if(key_buffer.cnt < SIZE_KEYBOARD_BUFFER - 1){
                if (key_buffer.enable){
                    key_buffer.buffer[key_buffer.cnt++] = key_char;
                }
                putc(key_char);
            }
            
            goto END;
        }
        // cases for special keys
        switch (char_index){
            case ENTER:
                if(key_buffer.cnt < SIZE_KEYBOARD_BUFFER){
                    if (key_buffer.enable){
                        key_buffer.buffer[key_buffer.cnt++] = '\n';
                    }
                    putc('\n');
                }
                break;
            case TAB:               // put 4 space 
                for(i = 0; i < 4; i++){
                    if(key_buffer.cnt >= SIZE_KEYBOARD_BUFFER - 1){
                        break;
                    }
                    if (key_buffer.enable){
                        key_buffer.buffer[key_buffer.cnt++] = ' ';
                    }
                    putc(' ');
                }
                break;
            case SHIFT:
                key_status.shift = 1;
                break;
            case SHIFT_R:
                key_status.shift_r = 1;
                break;
            case ALT:
                key_status.alt = 1;
                break;
            case CTRL:
                key_status.ctrl = 1;
                break;
            case CAPSLOCK:          // change the capital state
                key_status.capital_status ^= 1;
                break;
            case BACKSPACE:
                if (key_buffer.enable){
                    if (key_buffer.cnt > 0){
                        key_buffer.cnt--;
                        removec();
                    }
                }
                break;
            default:
                break;
        }
    }
    // while(1){
    //     // Check whether the input value is in range for reasonable key
    //     if((inb(KEY_DATAPORT) > 0) && (inb(KEY_DATAPORT) < KEY_NUM)){
    //         pressed_key = inb(KEY_DATAPORT);
    //         break;
    //     }
    // }

END:
    send_eoi(IRQ_KEYBOARD);
    sti();
    return;
}

/*
 *	Function: keyboard_open
 *	Description: Enable the keyboard operations
 *	inputs:		None
 *	outputs:	0 for success
 *	effects:	None
 */
int32_t keyboard_open(){
    enable_irq(IRQ_KEYBOARD);
    return 0;
}

/*
 *	Function: terminal_open
 *	Description: Enable and initialize the terminal
 *	inputs:		filename - name of the file
 *	outputs:	0 for success
 *	effects:	None
 */
int32_t terminal_open(const uint8_t* filename){
    key_buffer.cnt = 0;
    key_buffer.enable = 0;
    return 0;
}

/*
 *	Function: terminal_read
 *	Description: This function reads for the terminal from a provided buffer,
 *               and return the number of read bytes
 *	inputs:		fd - fd from the main function of ece391ls.c
 *              buffer - provided buffer, include the content to be read
 *              nbytes - Number of bytes to be read
 *	outputs:	None
 *	effects:	Include buffer contents copy
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    int32_t nbytes_read;
    key_buffer.cnt = 0;
    key_buffer.enable = 1;
    char* buffer = (char*) buf;
    if (buffer == NULL){
        printf("terminal_read: null buffer\n");
        return -1;
    }
    while(1){
        if(key_buffer.cnt > 0){
            if(key_buffer.buffer[key_buffer.cnt - 1] == '\n'){
                break;
            }
        }
    }
    if (nbytes > 128){
        printf("Can only read 128 bytes.\n");
    }
    nbytes_read = (key_buffer.cnt > nbytes) ? nbytes : key_buffer.cnt;
    memcpy(buffer, key_buffer.buffer, nbytes_read);
    key_buffer.cnt = 0;
    key_buffer.enable = 0;
    return nbytes_read;
}

/*
 *	Function: terminal_write
 *	Description: This function writes for the terminal to a provided buffer,
 *               and return the number of written bytes
 *	inputs:		fd - fd from the main function of ece391ls.c
 *              buffer - provided buffer to be written into
 *              nbytes - Number of bytes to be write
 *	outputs:	None
 *	effects:	Call the putc function
 */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    int i;
    int32_t nbytes_write = 0;
    char* buffer = (char*) buf;
    if (buffer == NULL){
        printf("terminal_write: null buffer\n");
        return -1;
    }
    for (i = 0; i < nbytes; i++){
        if(buffer[i] == '\0'){
            //break;
        }
        putc(buffer[i]);
        nbytes_write++;
    }
    return nbytes_write;
}

/*
 *	Function: terminal_close
 *	Description: This function closes the terminal
 *	inputs:		fd - fd from the main function of ece391ls.c
 *	outputs:	0 for success
 *	effects:	None
 */
int32_t terminal_close(int32_t fd){
    key_buffer.cnt = 0;
    key_buffer.enable = 0;
    return -1;
}
