 
// Chaidhat Chaimongkol's MinimalistiC Compiler
// ARM64 output - 16:10:21 May 27 2026
 
.section __TEXT,__text
.p2align 2
.globl _add
.p2align 2
_add:
    stp x29, x30, [sp, #-32]!
    mov x29, sp
    // function body
    str x0, [x29, #16]
    str x1, [x29, #24]
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    ldr x9, [x29, #24]
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    mov x0, x9
    b .L_add_epilogue
.L_add_epilogue:
    ldp x29, x30, [sp], #32
    ret
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
    str x9, [sp, #-16]!
    mov x9, #7
    mov x10, x9
    ldr x9, [sp], #16
    cmp x9, x10
    cset x9, gt
    cmp x9, #0
    b.eq .L_endif_0
    mov x9, #1
    str x9, [x29, #32]
.L_endif_0:
.L_while_top_1:
    ldr x9, [x29, #32]
    str x9, [sp, #-16]!
    mov x9, #0
    mov x10, x9
    ldr x9, [sp], #16
    cmp x9, x10
    cset x9, gt
    cmp x9, #0
    b.eq .L_while_end_1
    ldr x9, [x29, #32]
    str x9, [sp, #-16]!
    mov x9, #1
    mov x10, x9
    ldr x9, [sp], #16
    sub x9, x9, x10
    str x9, [x29, #32]
    b .L_while_top_1
.L_while_end_1:
    mov x9, #10
    str x9, [sp, #-16]!
    mov x9, #20
    str x9, [sp, #-16]!
    ldr x1, [sp], #16
    ldr x0, [sp], #16
    bl _add
    mov x9, x0
    str x9, [x29, #40]
    ldr x9, [x29, #32]
    mov x0, x9
    b .L_main_epilogue
.L_main_epilogue:
    ldp x29, x30, [sp], #48
    ret
 
