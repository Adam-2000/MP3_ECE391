#include "rtc.h"

/* void RTC_INIT(void) 
    Inputs: none
    Outputs: none 
    func: init the rtc 
*/
volatile uint32_t rtc_counter;

void RTC_INIT(void)
{
    uint8_t prev;
    cli();
    /* frequency define */
    /* select the Register A and disable NMI*/
    outb(REGA_OFF, PORT_70);
    outb(0x20,PORT_CMOS); //???????????????????
    /* read the current value in Reg_A */
    outb(REGA_OFF, PORT_70);
    prev = inb(PORT_CMOS);
    /* since inb would move RegD so agagin oub */
    outb(REGA_OFF, PORT_70);
    outb(((prev & 0xF0) | INIT_FREQ), PORT_CMOS);

    /* select the Register B and disable NMI*/
    outb((REGB_OFF | RTC_NMI), PORT_70);

    /* read the current value in Reg_B */
    prev = inb(PORT_CMOS);

    /* since inb would move RegD so agagin oub */
    outb((REGB_OFF | RTC_NMI), PORT_70);
    outb((prev | RTC_PIE), PORT_CMOS);

    sti();
    enable_irq(IRQ_RTC_NUM);
    
}


/*  void RTC_handler(void)
    Inputs: none
    Outputs: none
    func: handle the interrputs after called
*/
void RTC_handler(void)
{
    //printf("int rtc handler: %d\n", rtc_counter);
    /* connect the port of RegC */
    outb(REGC_OFF,PORT_70);
    /* just read the data */
    inb(PORT_CMOS);
    //rtc_counter++;
    test_interrupts();
    send_eoi(IRQ_RTC_NUM);

}

