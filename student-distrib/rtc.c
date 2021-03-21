#include "rtc.h"
#include "lib.h"


/* void RTC_INIT(void) 
    Inputs: none
    Outputs: none 
    func: init the rtc */

void RTC_INIT(void)
{
    uint32_t flags;
    cli_and_save(flags);

    /* select the Register B and disable NMI*/
    outb((REGB_OFF | RTC_NMI), PORT_70);

    /* read the current value in Reg_B */
    unsigned char prev = inb(PORT_CMOS);

    /* since inb would move RegD so agagin oub */
    outb((REGB_OFF | RTC_NMI), PORT_70);
    outb((prev | RTC_PIE), PORT_CMOS);


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