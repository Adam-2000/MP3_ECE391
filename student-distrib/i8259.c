/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = FB_MASK; /* IRQs 0-7  */
uint8_t slave_mask = FF_MASK;  /* IRQs 8-15 */



/* void i8259_init(void) */
/* Inputs: none */
/* Outputs: none */
/* connects the ports of master's and slave's data ports */
/* Initialize the 8259 PIC */
void i8259_init(void) 
{
    /* mask all of 8259A-1/2 */
    outb(FF_MASK, MASTER_8259_DATA); // left for the port 2 for slave
    outb(FF_MASK, SLAVE_8259_DATA);

    outb(ICW1, MASTER_8259_PORT); // ICW1 select 8259A-1 Init
    outb(ICW2_MASTER, MASTER_8259_DATA); // ICW2 IR0-7 map to 0x20-0x27
    outb(ICW3_MASTER, MASTER_8259_DATA); // has a slave on IR2
    outb(ICW4, MASTER_8259_DATA); // normal EOI

    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_SLAVE, SLAVE_8259_DATA);
    outb(ICW3_SLAVE, SLAVE_8259_DATA); // slave is on IR2
    outb(ICW4, SLAVE_8259_DATA); // normal EOI

}


/* void enable_irq(irq_num) */
/* Inputs: irq_num - the number of the interrupt */
/* Outputs: Writing the interrupt bit in slave and master */
/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) 
{
    /* depending on the irq_num master/slave/none */
    if( (irq_num>=IRQ_MIN) && (irq_num<=IRQ_MASTER) )
    {
        /* master PIC */
        uint8_t bit_choice = 1 << irq_num;
        bit_choice ~= bit_choice;
        master_mask &= bit_choice;
        outb(master_mask, MASTER_8259_DATA);
    }
    else if( (irq_num>IRQ_MASTER) && (irq_num<IRQ_MAX) )
    {
        /* Slave PIC */
        uint32_t t_irq = irq_num - IRQ_MASTER - 1;
        uint8_t bit_choice = 1 << t_irq;
        bit_choice ~= bit_choice;
        slave_mask &= bit_choice;
        outb(slave_mask,SLAVE_8259_DATA);
    }
    else
    {
        return;
    }

}


/* void disable_irq(uint32_t irq_num)*/
/* Inputs: irq_num - the number of the interrupt */
/* Outputs: Writing the interrupt bit in slave and master */
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) 
{
    /* depending on the irq_num master/slave/none */
    if( (irq_num>=IRQ_MIN) && (irq_num<=IRQ_MASTER) )
    {
        /* master PIC */
        uint8_t bit_choice = 1 << irq_num;
        master_mask |= bit_choice;
        outb(master_mask, MASTER_8259_DATA);
    }
    else if( (irq_num>IRQ_MASTER) && (irq_num<IRQ_MAX) )
    {
        /* Slave PIC */
        uint32_t t_irq = irq_num - IRQ_MASTER - 1;
        uint8_t bit_choice = 1 << irq_num;
        slave_mask |= bit_choice;
        outb(slave_mask,SLAVE_8259_DATA);
    }
    else
    {
        return;
    }
}


/* Inputs: irq number */
/* Outputs: none */
/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) 
{
    /* depending on the irq_num master/slave/none */
    if( (irq_num>=IRQ_MIN) && (irq_num<=IRQ_MASTER) )
    {
        /* it is the master branch */
        outb( (EOI | irq_num), MASTER_8259_PORT );
    }
    else if( (irq_num>IRQ_MASTER) && (irq_num<IRQ_MAX) )
    {
        uint32_t t_irq = irq_num - IRQ_MASTER - 1; // this is the port value for slave 
        outb( (EOI | t_irq), SLAVE_8259_PORT);
        outb( (EOI | SM_SELECT), MASTER_8259_PORT );
    }
    else
    {
        return;
    }

}
