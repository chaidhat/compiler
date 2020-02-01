 
# Chaidhat Chaimongkol's #
# MinimalistiC Compiler  #
# on 11:49:50 Feb  1 2020 #
 
.file   "asm_standard_test.mc"
 
# global declarations
.data
 
# global function declarations
.text
.globl _main
_main:
    # subroutine prologue
    pushl   %ebp
    movl    %esp, %ebp
    # subroutine main
    pushl   $0
    movl    $0, %eax
    # subroutine epilogue
    movl    %ebp, %esp
    popl    %ebp
    ret
 
