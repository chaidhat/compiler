 
# Chaidhat Chaimongkol's #
# MinimalistiC Compiler  #
# on 10:13:23 Feb  8 2020 #
 
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
    movl    -4(%ebp), %ebx
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
 
