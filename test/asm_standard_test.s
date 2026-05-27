 
// Chaidhat Chaimongkol's MinimalistiC Compiler
// ARM64 output - 16:13:47 May 27 2026
 
.section __TEXT,__text
.p2align 2
.globl _main
.p2align 2
_main:
    stp x29, x30, [sp, #-32]!
    mov x29, sp
    // function body
    mov x9, #0
    str x9, [x29, #16]
.L_main_epilogue:
    ldp x29, x30, [sp], #32
    ret
 
