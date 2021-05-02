
#define ASM 1
#define ESP_PCB_MASK 0xFFFFE000
#define FIRST_PCB_PTR 0x7FE000
#include "system_calls.h"
.data
    EIP_VAL = 0
    ESP_VAL = 4
    EBP_VAL = 8
    PARENT_PCB_PTR = 12
    FDA = 16

.global execute_handler, halt_handler, set3shells, PIT_handler

execute_handler:
    pushl %ebp
    movl  %esp, %ebp
    # parse args
    movl  8(%ebp), %ebx
    pushl %ebx
    call execute_helper
    addl $4, %esp
    # get new pcb
    cmpl $-1, %eax
    je end_execute_handler
    movl %eax, %edi
    # get old pcb
    movl $ESP_PCB_MASK, %esi
    andl %esp, %esi
    # store reg value to old pcb
    movl $check_bottom_shell, EIP_VAL(%esi)
    movl %esp, ESP_VAL(%esi)
    movl %ebp, EBP_VAL(%esi)
fake_iret:
    # push iret context
    pushl $USER_DS
    pushl ESP_VAL(%edi)
    pushfl
    pushl $USER_CS
    pushl EIP_VAL(%edi)
    IRET
check_bottom_shell:
    movl $ESP_PCB_MASK, %esi
    andl %esp, %esi
    cmpl $FIRST_PCB_PTR, %esi
    je execute_shell
end_execute_handler:
    leave
    ret


set3shells:
    pushl %ebp
    movl  %esp, %ebp
    call set3shells_helper
    # get old pcb
    movl $ESP_PCB_MASK, %esi
    andl %esp, %esi
    # store reg value to old pcb
    movl $check_bottom_shell, EIP_VAL(%esi)
    movl %esp, ESP_VAL(%esi)
    movl %ebp, EBP_VAL(%esi)
    movl $0, start_flag
    leave
    ret

start_flag:
    .long 0
visit_flag:
    .long 0

PIT_handler:
    pushl %ebp
    movl  %esp, %ebp
    call PIT_handler_helper
    # get new pcb
    cmpl $0, %eax
    je end_PIT_handler
    movl %eax, %edi

    // check if no terminals have been opened
    cmpl $0, start_flag
    jne PIT_save
    sti
    movl $1, start_flag
    jmp fake_iret
    
PIT_save:
    # get old pcb
    movl $ESP_PCB_MASK, %esi
    andl %esp, %esi
    # store reg value to old pcb
    movl $end_PIT_handler, EIP_VAL(%esi)
    movl %esp, ESP_VAL(%esi)
    movl %ebp, EBP_VAL(%esi)

    // check if we need to open a new terminal
    pushl %ebx
    movl visit_flag, %ebx
    # because we have 2 more terminals to open
    cmpl $2, %ebx
    je PIT_restore
    addl $1, %ebx
    movl %ebx, visit_flag
    popl %ebx
    sti
    jmp fake_iret

PIT_restore:
    popl %ebx
    # retore next pcb info
    movl EBP_VAL(%edi), %ebp
    movl ESP_VAL(%edi), %esp
    jmp *EIP_VAL(%edi)

end_PIT_handler:
    leave
    ret





halt_handler:
    pushl %ebp
    movl %esp, %ebp
    // push arg
    movl 8(%esp), %ebx
    pushl %ebx
    call halt_helper
    addl $4, %esp
    // get new regiser values
    movl $ESP_PCB_MASK, %esi
    andl %esp, %esi
    movl ESP_VAL(%esi), %esp
    movl EBP_VAL(%esi), %ebp
    jmp *EIP_VAL(%esi)
    leave
    ret

#define DO_CALL(name,number)   \
.GLOBL name                   ;\
name:   PUSHL	%EBX          ;\
	MOVL	$number,%EAX  ;\
	MOVL	8(%ESP),%EBX  ;\
	MOVL	12(%ESP),%ECX ;\
	MOVL	16(%ESP),%EDX ;\
	INT	$0x80         ;\
	POPL	%EBX          ;\
	RET

/* the system call library wrappers */
DO_CALL(halt,SYS_HALT)
DO_CALL(execute,SYS_EXECUTE)
DO_CALL(read,SYS_READ)
DO_CALL(write,SYS_WRITE)
DO_CALL(open,SYS_OPEN)
DO_CALL(close,SYS_CLOSE)
DO_CALL(getargs,SYS_GETARGS)
DO_CALL(vidmap,SYS_VIDMAP)
DO_CALL(set_handler,SYS_SET_HANDLER)
DO_CALL(sigreturn,SYS_SIGRETURN)
