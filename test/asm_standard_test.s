 
# Chaidhat Chaimongkol's #
# MinimalistiC Compiler  #
# on 17:13:05 Feb  6 2020 #
 
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
    # subroutine return
    popl    %eax
    jmpl    *%eax
.globl _foo
_foo:
    # subroutine prologue
    pushl   %ebp
    movl    %esp, %ebp
    # subroutine main
    # subroutine epilogue
    movl    %ebp, %esp
    popl    %ebp
    # subroutine return
    popl    %eax
    jmpl    *%eax
 
