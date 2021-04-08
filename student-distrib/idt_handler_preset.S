#define ASM 1

#define push_all_reg \
    pushl %fs   ;\
    pushl %es   ;\
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
    popl %esi  ;\
    popl %edi  ;\
    popl %ebp   ;\
    popl %eax   ;\
    popl %ds    ;\
    popl %es    ;\
    popl %fs    ;\

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

//handlr_1_interrput(timer_chip,      timer_chip_func)
handlr_1_interrput(keyboard,        keyboard_handler)
//handlr_1_interrput(serial_port,     serial_port_func)
handlr_1_interrput(real_time_clock, RTC_handler)
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
handlr_1_exception(general_protection,      exception_general_protection)
handlr_1_exception(page_fault,              exception_page_fault)
handlr_1_exception(x87_fpu_floating_point_error, exception_x87_fpu_floating_point_error)
handlr_1_exception(alignment_check,         exception_alignment_check)
handlr_1_exception(machine_check,           exception_machine_check)
handlr_1_exception(simd_floating_point,     exception_simd_floating_point)

.globl system_call
system_call:              
    pushl %ebp
    pushl %esi
    pushl %edi
    // check for valid system call index
    cmpl $1, %eax
    jb invalid_system_call
    cmpl $10, %eax
    ja invalid_system_call
    // push three arguments
    pushl %edx
    pushl %ecx
    pushl %ebx
    call *syscall_jumptable - 4(, %eax, 4)
    popl %ebx
    popl %ecx
    popl %edx  
    jmp end_system_call
invalid_system_call:
    movl $-1, %eax
end_system_call:
    popl %edi  
    popl %esi
    popl %ebp  
    iret

syscall_jumptable:
    .long halt_handler, execute_handler, read_handler, write_handler, open_handler, close_handler
    .long getargs_handler, vidmap_handler, set_handler, sigreturn_handler

getargs_handler:
vidmap_handler:
set_handler_handler:
sigreturn_handler:
    .long 0 // padding