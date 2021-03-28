#include "rtc.h"

/* void RTC_INIT(void) 
    Inputs: none
    Outputs: none 
    func: init the rtc 
*/
//volatile uint32_t rtc_counter;
static volatile uint32_t rtc_counter;

int32_t rtc_open()
{
    uint8_t prev;
    cli();

    rtc_write(RTC_INIT_F);
    /* select the Register B and disable NMI*/
    outb((REGB_OFF | RTC_NMI), PORT_70);

    /* read the current value in Reg_B */
    prev = inb(PORT_CMOS);

    /* since inb would move RegD so agagin oub */
    outb((REGB_OFF | RTC_NMI), PORT_70);
    outb((prev | RTC_PIE), PORT_CMOS);

    sti();
    enable_irq(IRQ_RTC_NUM);
    return 0;
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
    // counter counts up
    rtc_counter++;
    rtc_counter %= CNT_MAX;
    //test_interrupts();
    send_eoi(IRQ_RTC_NUM);

}

/* int32_t rtc_write()
 * Inputs: lg_freq -- the lg(frequency)
 * Outputs: none
 * We need to write the frequency to the RTC, and return 0/-1 to see if okay
 * the frequency set
 */
int32_t rtc_write(int32_t lg_freq)
{
    uint32_t prev;
    // don't accept frequecy larger than 1024Hz
    if (lg_freq > 10){
        return -1;
    }
    // argument to rtc chip
    uint8_t rate = FREQ2RATE(lg_freq);
    /* read the current value in Reg_A */
    outb(REGA_OFF, PORT_70);
    prev = inb(PORT_CMOS);
    /* since inb would move RegD so agagin oub */
    outb(REGA_OFF, PORT_70);
    outb(((prev & 0xF0) | rate), PORT_CMOS);
    return 0;
}

/* int32_t rtc_read() : return until next rtc interrupt
 * Inputs: none
 * Outputs: none
 * Side effect: nothing
 */
int32_t rtc_read(){
    int32_t old_val = rtc_counter;
    while (old_val == rtc_counter);
    return 0;
}

/* int32_t rtc_close()
 * Inputs: none
 * Outputs: none
 * Side effect: nothing
 */
int32_t rtc_close(){
    return 0;
}
