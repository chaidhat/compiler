 
// Chaidhat Chaimongkol's MinimalistiC Compiler
// ARM64 output - 16:01:50 May 27 2026
 
.section __TEXT,__text
.p2align 2
.globl _main
.p2align 2
_main:
    stp x29, x30, [sp, #-48]!
    mov x29, sp
    // function body
    mov x9, #5
    str x9, [x29, #16]
    mov x9, #3
    str x9, [x29, #24]
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    ldr x9, [x29, #24]
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    str x9, [x29, #32]
    ldr x9, [x29, #32]
    mov x0, x9
    ldp x29, x30, [sp], #48
    ret
 
