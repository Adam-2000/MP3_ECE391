#define ASM 1

#define push_all_reg \
    pushw $0    ;\
    pushl %fs   ;\
    pushw $0    ;\
    pushl %es   ;\
    pushw $0    ;\
    pushl %ds   ;\
    pushl %eax  ;\
    pushl %ebp  ;\
    pushl %edi  ;\
    pushl %esi  ;\
    pushl %edx  ;\
    pushl %ecx  ;\
    pushl %ebx 

#define pop_all_reg \
    popl %ebx   ;\
    popl %ecx   ;\
    popl %edx   ;\
    pushl %esi  ;\
    pushl %edi  ;\
    popl %ebp   ;\
    popl %eax   ;\
    popw %ds    ;\
    addl $2, %esp   ;\
    popw %es    ;\
    addl $2, %esp   ;\
    popw %fs    ;\
    addl $2, %esp 

#define handlr_1_interrput(name, function)    \
    .globl name                            ;\
name:                                  ;\
    cli                 ;\
    push_all_reg    ;\
    call function   ;\
    pop_all_reg     ;\
    sti         ;\
    iret 

#define handlr_1_exception(name, function)  \
    .globl name                            ;\
name:                                   ;\
    cli                 ;\
    push_all_reg    ;\
    call function    ;\
    pop_all_reg     ;\
    sti             ;\
    iret


.globl system_call
system_call:
    cli                 
    push_all_reg    
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    call system_call_func   
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx 
    pop_all_reg     
    sti             
    iret

//handlr_1_interrput(timer_chip,      timer_chip_func)
handlr_1_interrput(keyboard,        keyboard_handler)
//handlr_1_interrput(serial_port,     serial_port_func)
handlr_1_interrput(real_time_clock, real_time_clock_func)
//handlr_1_interrput(eth0,            eth0_func)
//handlr_1_interrput(PS_2,            PS_2_func)
//handlr_1_interrput(ide0,            ide0_func)

handlr_1_exception(divide_error,            exception_divide_error)
handlr_1_exception(debug,                   exception_debug)
handlr_1_exception(nmi_interrupt,           exception_nmi_interrupt)
handlr_1_exception(breakpoint,              exception_breakpoint)
handlr_1_exception(overflow,                exception_overflow)
handlr_1_exception(bound_range_exceeded,    exception_bound_range_exceeded)
handlr_1_exception(invalid_opcode,          exception_invalid_opcode)
handlr_1_exception(device_not_available,    exception_device_not_available)
handlr_1_exception(double_fault,            exception_double_fault)
handlr_1_exception(invalid_tss,             exception_invalid_tss)
handlr_1_exception(segment_not_present,     exception_segment_not_present)
handlr_1_exception(stack_segment_fault,     exception_stack_segment_fault)
handlr_1_exception(general_protection,      exception_stack_general_protection)
handlr_1_exception(page_fault,              exception_page_fault)
handlr_1_exception(x87_fpu_floating_point_error, exception_x87_fpu_floating_point_error)
handlr_1_exception(alignment_check,         exception_alignment_check)
handlr_1_exception(machine_check,           exception_machine_check)
handlr_1_exception(simd_floating_point,     exception_simd_floating_point)

