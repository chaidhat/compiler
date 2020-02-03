 
# Chaidhat Chaimongkol's #
# MinimalistiC Compiler  #
# on 20:43:02 Feb  2 2020 #
 
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
.globl _foo
_foo:
    # subroutine prologue
    pushl   %ebp
    movl    %esp, %ebp
    # subroutine main
    # subroutine epilogue
    movl    %ebp, %esp
    popl    %ebp
    ret
 
