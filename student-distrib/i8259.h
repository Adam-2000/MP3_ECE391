/* i8259.h - Defines used in interactions with the 8259 interrupt
 * controller
 * vim:ts=4 noexpandtab
 */

#ifndef _I8259_H
#define _I8259_H

#include "types.h"

/* Ports that each PIC sits on */
#define MASTER_8259_PORT    0x20
#define MASTER_8259_DATA    0x21
#define SLAVE_8259_PORT     0xA0
#define SLAVE_8259_DATA     0xA1

/* Initialization control words to init each PIC.
 * See the Intel manuals for details on the meaning
 * of each word */
#define ICW1                0x11
#define ICW2_MASTER         0x20
#define ICW2_SLAVE          0x28
#define ICW3_MASTER         0x04
#define ICW3_SLAVE          0x02
#define ICW4                0x01

/* End-of-interrupt byte.  This gets OR'd with
 * the interrupt number and sent out to the PIC
 * to declare the interrupt finished */
#define EOI                 0x60

#define IRQ_RTC_NUM         8
#define IRQ_KEYBOARD	    1 
#define IRQ_TIMER_CHIP      0

/* CSJ's add */
#define IRQ_MAX             15
#define IRQ_MIN             0
#define IRQ_MASTER          7
#define SM_SELECT           0x02
#define FF_MASK             0xff
#define FB_MASK             0xfb



#ifndef ASM
/* Initialize both PICs */
void i8259_init(void);
/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num);
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num);
/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num);
#endif

/* Externally-visible functions */


#endif /* _I8259_H */
