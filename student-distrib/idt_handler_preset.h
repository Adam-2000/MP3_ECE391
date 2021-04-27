#ifndef _IDT_HANDLER_PRESET_H
#define _IDT_HANDLER_PRESET_H

#ifndef ASM
/* functions that put in the idt table as function pointers*/
/*exceptions*/
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

/*interrupts*/
void timer_chip();
void keyboard();
void real_time_clock();

/*system call function pointer in the idt*/
void system_call();

// extern int32_t halt_wrapper(uint8_t status);
// extern int32_t execute_wrapper(const uint8_t* command);
// extern int32_t read_wrapper(int32_t fd, void* buf, int32_t nbytes);
// extern int32_t write_wrapper(int32_t fd, const void* buf, int32_t nbytes);
// extern int32_t open_wrapper(const uint8_t* filename);
// extern int32_t close_wrapper(int32_t fd);

// extern int32_t halt(uint8_t status);
// extern int32_t execute(const uint8_t* command);
// extern int32_t read(int32_t fd, void* buf, int32_t nbytes);
// extern int32_t write(int32_t fd, const void* buf, int32_t nbytes);
// extern int32_t open(const uint8_t* filename);
// extern int32_t close(int32_t fd);
#endif


#endif
