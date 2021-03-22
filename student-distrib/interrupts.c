#include "interrupts.h"

void keyboard_func(){
    printf("interrupt: keyboard interrupt");
}
void real_time_clock_func(){
    printf("interrupt: rtc interrupt");
}
