#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "exceptions.h"
#include "system_calls.h"
#ifndef ASM
/*
 *	Function: exception_divide_error
 *	Description: Raise the exception when divide error is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: divide error"
 */
void exception_divide_error(){
    printf("Exception: divide error");
    halt(0);
}

/*
 *	Function: exception_debug
 *	Description: Raise the exception when something wrong with the debug module is found
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: debug"
 */
void exception_debug(){
    printf("Exception: debug");
    halt(0);
}

/*
 *	Function: exception_nmi_interrupt
 *	Description: Raise the exception when nmi interrupt is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: nmi interrupt"
 */
void exception_nmi_interrupt(){
    printf("Exception: nmi interrupt");
    halt(0);
}

/*
 *	Function: exception_breakpoint
 *	Description: Raise the exception when breakpoint is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: breakpoint"
 */
void exception_breakpoint(){
    printf("Exception: breakpoint");
    halt(0);
}

/*
 *	Function: exception_overflow
 *	Description: Raise the exception when overflow is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: overflow"
 */
void exception_overflow(){
    printf("Exception: overflow");
    halt(0);
}

/*
 *	Function: exception_bound_range_exceeded
 *	Description: Raise the exception when bound range exceeded is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: bound range exceeded"
 */
void exception_bound_range_exceeded(){
    printf("Exception: bound range exceeded");
    halt(0);
}

/*
 *	Function: exception_invalid_opcode
 *	Description: Raise the exception when invalid opcode is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: invalid opcode"
 */
void exception_invalid_opcode(){
    printf("Exception: invalid opcode");
    halt(0);
}

/*
 *	Function: exception_device_not_available
 *	Description: Raise the exception when device is not available
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: device not available"
 */
void exception_device_not_available(){
    printf("Exception: device not available");
    halt(0);
}

/*
 *	Function: exception_double_fault
 *	Description: Raise the exception when double fault is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: double fault"
 */
void exception_double_fault(){
    printf("Exception: double fault");
    halt(0);
}

/*
 *	Function: exception_invalid_tss
 *	Description: Raise the exception when invalid tss is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: invalid tss"
 */
void exception_invalid_tss(){
    printf("Exception: invalid tss");
    halt(0);
}

/*
 *	Function: exception_segment_not_present
 *	Description: Raise the exception when segment is not presented
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: segment not present"
 */
void exception_segment_not_present(){
    printf("Exception: segment not present");
    halt(0);
}

/*
 *	Function: exception_stack_segment_fault
 *	Description: Raise the exception when stack segment fault takes place
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: stack segment fault"
 */
void exception_stack_segment_fault(){
    printf("Exception: stack segment fault");
    halt(0);
}

/*
 *	Function: exception_general_protection
 *	Description: Raise the exception when general protection takes place
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: general protection"
 */
void exception_general_protection(){
    printf("Exception: general protection");
    halt(0);
}

/*
 *	Function: exception_page_fault
 *	Description: Raise the exception when page fault is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: page fault"
 */
void exception_page_fault(){
    printf("Exception: page fault");
    halt(0);
}

/*
 *	Function: exception_x87_fpu_floating_point_error
 *	Description: Raise the exception when x87 fpu floating point error is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: x87 fpu floating point error"
 */
void exception_x87_fpu_floating_point_error(){
    printf("Exception: x87 fpu floating point error");
    halt(0);
}

/*
 *	Function: exception_alignment_check
 *	Description: Raise the exception when alignment check error is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: alignment check"
 */
void exception_alignment_check(){
    printf("Exception: alignment check");
    halt(0);
}

/*
 *	Function: exception_machine_check
 *	Description: Raise the exception when machine check error is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: machine check"
 */
void exception_machine_check(){
    printf("Exception: machine check");
    halt(0);
}

/*
 *	Function: exception_simd_floating_point
 *	Description: Raise the exception when simd floating point is detected
 *	inputs:		None
 *	outputs:	None
 *	effects:	Raise exception and print corresponding error info "Exception: simd floating point"
 */
void exception_simd_floating_point(){
    printf("Exception: simd floating point");
    halt(0);
}
#endif
#endif

