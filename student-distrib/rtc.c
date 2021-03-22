#include "rtc.h"

/* void RTC_INIT(void) 
    Inputs: none
    Outputs: none 
    func: init the rtc 
*/
void RTC_INIT(void)
{
    uint32_t flags;
    uint8_t prev;
    cli_and_save(flags);
    /* frequency define */
    /* select the Register A and disable NMI*/
    outb((REGA_OFF | RTC_NMI), PORT_70);

    /* read the current value in Reg_A */
    prev = inb(PORT_CMOS);

    /* since inb would move RegD so agagin oub */
    outb((REGA_OFF | RTC_NMI), PORT_70);
    outb((prev | INIT_FREQ), PORT_CMOS);

    /* select the Register B and disable NMI*/
    outb((REGB_OFF | RTC_NMI), PORT_70);

    /* read the current value in Reg_B */
    prev = inb(PORT_CMOS);

    /* since inb would move RegD so agagin oub */
    outb((REGB_OFF | RTC_NMI), PORT_70);
    outb((prev | RTC_PIE), PORT_CMOS);

    
    enable_irq(IRQ_RTC_NUM);
    sti();
    restore_flags(flags);
}


/*  void RTC_handler(void)
    Inputs: none
    Outputs: none
    func: handle the interrputs after called
*/
void RTC_handler(void)
{
    /* connect the port of RegC */
    outb(REGC_OFF,PORT_70);
    /* just read the data */
    inb(PORT_CMOS);

    send_eoi(IRQ_RTC_NUM);
}



/* void RTC_frequency_set */
/* Inputs: the settled frequency required */
/* Outputs: none */
/* func: modify the required rtc frequency in given */
/*void RTC_frequency_set(uint32_t f)
{


}
*/
