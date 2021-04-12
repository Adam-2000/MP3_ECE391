#ifndef _PAGING_ENABLE_H
#define _PAGING_ENABLE_H

#ifndef ASM
/*
 *  change_control_reg
 *   DESCRIPTION: set the correct control register values
 *   INPUTS: d: new cr3 value
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void change_control_reg(int d);

/*
 *  reload_cr3
 *   DESCRIPTION: set new cr3 value, flush TLB
 *   INPUTS: d: new cr3 value
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void reload_cr3(int d);
#endif
#endif

