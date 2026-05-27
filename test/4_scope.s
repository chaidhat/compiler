 
// Chaidhat Chaimongkol's MinimalistiC Compiler
// ARM64 output - 16:10:21 May 27 2026
 
.section __TEXT,__text
.p2align 2
.globl _a
.p2align 2
_a:
    stp x29, x30, [sp, #-32]!
    mov x29, sp
    // function body
    mov x9, #1
    str x9, [sp, #-16]!
    mov x9, #1
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    str x9, [x29, #16]
    mov x9, #2
    str x9, [x29, #16]
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    mov x9, #5
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    cmp x9, #0
    b.eq .L_endif_0
    mov x9, #5
    str x9, [x29, #16]
.L_endif_0:
.L_while_top_1:
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    mov x9, #5
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    cmp x9, #0
    b.eq .L_while_end_1
    mov x9, #5
    str x9, [x29, #16]
    b .L_while_top_1
.L_while_end_1:
    ldr x9, [x29, #0]
    str x9, [sp, #-16]!
    ldr x9, [x29, #0]
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    mov x0, x9
    b .L_a_epilogue
.L_a_epilogue:
    ldp x29, x30, [sp], #32
    ret
.globl _e
.p2align 2
_e:
    stp x29, x30, [sp, #-32]!
    mov x29, sp
    // function body
    bl _a
    mov x9, x0
    mov x9, #2
    str x9, [sp, #-16]!
    ldr x0, [sp], #16
    bl _c
    mov x9, x0
    mov x9, #0
    str x9, [x29, #16]
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    ldr x0, [sp], #16
    bl _c
    mov x9, x0
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    mov x9, #2
    str x9, [sp, #-16]!
    mov x9, #3
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    str x9, [sp, #-16]!
    ldr x0, [sp], #16
    bl _c
    mov x9, x0
    ldr x9, [x29, #16]
    str x9, [sp, #-16]!
    mov x9, #2
    str x9, [sp, #-16]!
    mov x9, #3
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    mov x10, x9
    ldr x9, [sp], #16
    add x9, x9, x10
    str x9, [sp, #-16]!
    mov x9, #2
    str x9, [sp, #-16]!
    ldr x1, [sp], #16
    ldr x0, [sp], #16
    bl _d
    mov x9, x0
    mov x9, #4
    str x9, [sp, #-16]!
    mov x9, #5
    str x9, [sp, #-16]!
    ldr x0, [sp], #16
    bl _c
    mov x9, x0
    str x9, [sp, #-16]!
    ldr x1, [sp], #16
    ldr x0, [sp], #16
    bl _d
    mov x9, x0
.L_e_epilogue:
    ldp x29, x30, [sp], #32
    ret
 
