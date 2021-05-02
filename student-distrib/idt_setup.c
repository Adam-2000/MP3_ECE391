#include "idt_setup.h"
/* 
 * idt_init
 * initialize the idt
 */
void idt_init(){
    int i;
    for (i = 0; i < NUM_VEC; i++){
        idt[i].seg_selector = KERNEL_CS;
        // unpresent as default, set_idt_entry() sets the bit
        idt[i].present = 0;
        // reserved default as trap type
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 1;
        idt[i].reserved4 = 0;
        // size 1 means 32bit interrupt gate
        idt[i].size = 1;
        // user previledge level default
        idt[i].dpl = 3;
    }
    // set exception
    for (i = 0; i < NUM_EXCEPTION; i++){
        // privilege level as 0 
        idt[i].dpl = 0;
    }
    // 0x09 and 0x0F reserved, 0x14~0x20 reserved
    SET_IDT_ENTRY(idt[0x00], divide_error            );
    SET_IDT_ENTRY(idt[0x01], debug        			 );           
    SET_IDT_ENTRY(idt[0x02], nmi_interrupt           );
    SET_IDT_ENTRY(idt[0x03], breakpoint              );
    SET_IDT_ENTRY(idt[0x04], overflow                );
    SET_IDT_ENTRY(idt[0x05], bound_range_exceeded    );
    SET_IDT_ENTRY(idt[0x06], invalid_opcode          );
    SET_IDT_ENTRY(idt[0x07], device_not_available    );
    SET_IDT_ENTRY(idt[0x08], double_fault            );
    SET_IDT_ENTRY(idt[0x0A], invalid_tss             );
    SET_IDT_ENTRY(idt[0x0B], segment_not_present     );
    SET_IDT_ENTRY(idt[0x0C], stack_segment_fault     );
    SET_IDT_ENTRY(idt[0x0D], general_protection      );
    SET_IDT_ENTRY(idt[0x0E], page_fault              );
    SET_IDT_ENTRY(idt[0x10], x87_fpu_floating_point_error);
    SET_IDT_ENTRY(idt[0x11], alignment_check         );
    SET_IDT_ENTRY(idt[0x12], machine_check           );
	SET_IDT_ENTRY(idt[0x13], simd_floating_point     );
    
    // set interrupt
    for (i = INTERRPUT_START_INDEX; i <= INTERRPUT_END_INDEX; i++){
        // privilege level as 0 
        idt[i].dpl = 0;
        // as interrupt type
        idt[i].reserved3 = 0;
    }
    SET_IDT_ENTRY(idt[TIMER_CHIP_INT_INDEX], timer_chip);
    SET_IDT_ENTRY(idt[KEYBOARD_INT_INDEX], keyboard);
	SET_IDT_ENTRY(idt[RTC_INT_INDEX], real_time_clock);
    // set system call
    SET_IDT_ENTRY(idt[SYS_CALL_INDEX], system_call);

}
