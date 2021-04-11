#ifndef _PAGING_ENABLE_H
#define _PAGING_ENABLE_H

#ifndef ASM

void change_control_reg(int d);
void reload_cr3(int d);
#endif
#endif

