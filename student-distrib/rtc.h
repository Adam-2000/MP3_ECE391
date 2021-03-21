/* rtc.h - Defines for rtc usage when interrupt or control*/

#ifndef _RTC_H
#define _RTC_H

#include "types.h"

#define REGA_OFF    0xA
#define REGB_OFF    0xB
#define REGC_OFF    0xC

#define PORT_70     0x70
#define PORT_CMOS   0x71

#define RTC_NMI     0x80
#define RTC_PIE     0x40

#define IRQ_RTC_NUM 8




/* init the rtc */
extern void RTC_INIT(void);

/* RTC handler */
extern void RTC_handler(void);


#endif 