#ifndef _IDT_HANDLER_PRESET_H
#define _IDT_HANDLER_PRESET_H

#ifndef ASM
void divide_error();            
void debug();                 
void nmi_interrupt();          
void breakpoint();             
void overflow();                
void bound_range_exceeded();    
void invalid_opcode();         
void device_not_available();    
void double_fault();            
void invalid_tss();             
void segment_not_present();     
void stack_segment_fault();     
void general_protection();      
void page_fault();              
void x87_fpu_floating_point_error();
void alignment_check();         
void machine_check();           
void simd_floating_point(); 

void keyboard();
void real_time_clock();

void system_call();

#endif


#endif
