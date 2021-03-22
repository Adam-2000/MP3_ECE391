#define ASM     1

.globl change_control_reg
.align 4
# do this process following the OSDev
change_control_reg:
  pushl %ebp
  movl  %esp, %ebp
  movl  8(%ebp), %eax     

  movl %eax, %cr3   # put directory into cr3

  movl %cr4, %eax
  orl  $0x10, %eax  # Sets the bit for using 4MB pages
  movl %eax, %cr4

  movl %cr0, %eax
  orl  $0x80000000, %eax  
  movl %eax, %cr0
  
  leave
  ret
