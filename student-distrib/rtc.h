/* rtc.h - Defines for rtc usage when interrupt or control*/

#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#include "i8259.h"

#define REGA_OFF    0xA
#define REGB_OFF    0xB
#define REGC_OFF    0xC

//#define INIT_FREQ   0xF
#define PORT_70     0x70
#define PORT_CMOS   0x71

#define RTC_NMI     0x80
#define RTC_PIE     0x40
#define CNT_MAX     0x80000000
#define IRQ_RTC_NUM 8

//#define RTC_INIT_F  1       // initial frequency, 2^1
//#define FREQ2RATE(freq) 16 - freq

#ifndef ASM

#include "lib.h"
//extern volatile uint32_t rtc_counter;

/* RTC handler */
extern void RTC_handler(void);
/* void RTC_frequency_set */
extern int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);
/* void RTC open */
extern int32_t rtc_open(const uint8_t* filename);
/* void RTC_close */
extern int32_t rtc_close(int32_t fd);
/* void RTC_frequency_set */
extern int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

#endif

#endif 

