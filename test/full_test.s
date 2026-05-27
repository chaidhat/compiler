 
// Chaidhat Chaimongkol's MinimalistiC Compiler
// ARM64 output - 16:13:47 May 27 2026
 
.section __TEXT,__text
.p2align 2
.globl _dot
.p2align 2
_dot:
    stp x29, x30, [sp, #-64]!
    mov x29, sp
    // function body
    str x0, [x29, #16]
    str x1, [x29, #24]
    str x2, [x29, #32]
    str x3, [x29, #40]
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    ldr x9, [x29, #32]
    mov x10, x9
    ldr x9, [sp], #16
    mul x9, x9, x10
    str x9, [sp, #-16]!
    ldr x9, [x29, #24]
    str x9, [sp, #-16]!
    ldr x9, [x29, #40]
    mov x10, x9
    ldr x9, [sp], #16
    mul x9, x9, x10
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    str x9, [x29, #48]
    ldr x9, [x29, #48]
    mov x0, x9
    b .L_dot_epilogue
.L_dot_epilogue:
    ldp x29, x30, [sp], #64
    ret
.globl _main
.p2align 2
_main:
    stp x29, x30, [sp, #-64]!
    mov x29, sp
    // function body
    mov x9, #3
    str x9, [x29, #16]
    mov x9, #4
    str x9, [x29, #24]
    mov x9, #1
    str x9, [x29, #32]
    mov x9, #2
    str x9, [x29, #40]
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    ldr x9, [x29, #24]
    str x9, [sp, #-16]!
    ldr x9, [x29, #32]
    str x9, [sp, #-16]!
    ldr x9, [x29, #40]
    str x9, [sp, #-16]!
    ldr x3, [sp], #16
    ldr x2, [sp], #16
    ldr x1, [sp], #16
    ldr x0, [sp], #16
    bl _dot
    mov x9, x0
    str x9, [x29, #48]
    mov x9, #10
    str x9, [x29, #56]
.L_while_top_0:
    ldr x9, [x29, #56]
    str x9, [sp, #-16]!
    mov x9, #0
    mov x10, x9
    ldr x9, [sp], #16
    cmp x9, x10
    cset x9, gt
    cmp x9, #0
    b.eq .L_while_end_0
    ldr x9, [x29, #56]
    str x9, [sp, #-16]!
    mov x9, #1
    mov x10, x9
    ldr x9, [sp], #16
    sub x9, x9, x10
    str x9, [x29, #56]
    b .L_while_top_0
.L_while_end_0:
    ldr x9, [x29, #48]
    str x9, [sp, #-16]!
    mov x9, #11
    str x9, [sp, #-16]!
    ldr x9, [x29, #56]
    str x9, [sp, #-16]!
    mov x9, #0
    mov x10, x9
    ldr x9, [sp], #16
    cmp x9, x10
    cset x9, eq
    mov x10, x9
    ldr x9, [sp], #16
    cmp x9, #0
    cset x9, ne
    cmp x10, #0
    cset x10, ne
    and x9, x9, x10
    mov x10, x9
    ldr x9, [sp], #16
    cmp x9, x10
    cset x9, eq
    cmp x9, #0
    b.eq .L_endif_1
    mov x9, #0
    mov x0, x9
    b .L_main_epilogue
.L_endif_1:
    mov x9, #1
    mov x0, x9
    b .L_main_epilogue
.L_main_epilogue:
    ldp x29, x30, [sp], #64
    ret
 
