#include "rtc.h"

/* void RTC_INIT(void) 
    Inputs: none
    Outputs: none 
    func: init the rtc 
*/
//volatile uint32_t rtc_counter;
static volatile uint32_t rtc_counter;

int32_t rtc_open(const uint8_t* filename)
{
    uint8_t prev;
    uint32_t init_lg_freq = RTC_INIT_F;
    cli();

    rtc_write(0, &init_lg_freq, 4);
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
 * Inputs: buf -- pointer to the lg(frequency)
 * Outputs: none
 * We need to write the frequency to the RTC, and return 0/-1 to see if okay
 * the frequency set
 */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes)
{
    uint32_t prev;
    uint32_t freq = *(uint32_t*)buf;
    uint8_t rate;
    // check 4 bytes
    if (nbytes != 4){
        return -1;
    }
    // look up table for frequency - rate
    switch (freq){
        case 2:
            rate = 0xF;
            break;
        case 4:
            rate = 0xE;
            break;
        case 8:
            rate = 0xD;
            break;
        case 16:
            rate = 0xC;
            break;
        case 32:
            rate = 0xB;
            break;
        case 64:
            rate = 0xA;
            break;
        case 128:
            rate = 0x9;
            break;
        case 256:
            rate = 0x8;
            break;
        case 512:
            rate = 0x7;
            break;
        case 1024:
            rate = 0x6;
            break;
        default:
            //don't accept other frequencies
            return -1;
    }
    // argument to rtc chip
    
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
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
    int32_t old_val = rtc_counter;
    while (old_val == rtc_counter);
    return 0;
}

/* int32_t rtc_close()
 * Inputs: none
 * Outputs: none
 * Side effect: nothing
 */
int32_t rtc_close(int32_t fd){
    return 0;
}
