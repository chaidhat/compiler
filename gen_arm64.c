#include "mcc.h"

#define genfileinserts(str) if( \
        mccstr(dest, destSz, "%s" str "\n", dest)) \
        { mccErr("gen insert error\n"); mccExit(1); }
#define genfileinsertm(str, ... ) if( \
        mccstr(dest, destSz, "%s" str "\n", dest, __VA_ARGS__)) \
        { mccErr("gen insert error\n"); mccExit(1); }
#define gfis(str) if( \
        mccstr(dest, destSz, "%s" str , dest)) \
        { mccErr("gen insert error\n"); mccExit(1); }
#define gfim(str, ... ) if( \
        mccstr(dest, destSz, "%s" str "", dest, __VA_ARGS__)) \
        { mccErr("gen insert error\n"); mccExit(1); }

static char currentEpilogueLabel[256];
static void genRoutine (char *dest, int destSz, IrRoutine *ir);
static void genInst (char *dest, int destSz, IrInst inst);

static char *regName (enum RegPhyType r)
{
    switch (r)
    {
        case RAT_x0:  return "x0";
        case RAT_x1:  return "x1";
        case RAT_x2:  return "x2";
        case RAT_x3:  return "x3";
        case RAT_x4:  return "x4";
        case RAT_x5:  return "x5";
        case RAT_x6:  return "x6";
        case RAT_x7:  return "x7";
        case RAT_x9:  return "x9";
        case RAT_x10: return "x10";
        case RAT_x11: return "x11";
        case RAT_sp:  return "sp";
        case RAT_fp:  return "x29";
        case RAT_lr:  return "x30";
    }
    return "x0";
}

static char *condName (enum CondCode c)
{
    switch (c)
    {
        case CC_EQ: return "eq";
        case CC_NE: return "ne";
        case CC_GT: return "gt";
        case CC_LT: return "lt";
        case CC_GE: return "ge";
        case CC_LE: return "le";
    }
    return "eq";
}

static void genOp (char *dest, int destSz, Operand op)
{
    switch (op.type)
    {
        case OT_num_lit:
            gfim("#%d", op.num);
            break;
        case OT_str_lit:
            gfim("%s", op.str);
            break;
        case OT_reg:
            if (op.reg.type == RT_phy) {
                gfim("%s", regName(op.reg.phy));
            } else if (op.reg.type == RT_rel) {
                gfim("[x29, #%d]", op.reg.rel);
            } else if (op.reg.type == RT_stat) {
                gfim("%s", op.reg.stat);
            }
            break;
        case OT_str_routine:
            gfim("%s", op.str);
            break;
        case OT_comment:
            gfim("// %s", op.str);
            break;
        default:
            break;
    }
}

static void genRoutine (char *dest, int destSz, IrRoutine *ir)
{
    mccLog("routine %s (frame %d)", ir->name, ir->frameSize);
    IrInst inst;
    int totalFrame = ir->frameSize;
    sprintf(currentEpilogueLabel, ".L_%s_epilogue", ir->name);

    genfileinsertm(".globl _%s", ir->name);
    genfileinserts(".p2align 2");
    genfileinsertm("_%s:", ir->name);

    /* prologue */
    genfileinsertm("    stp x29, x30, [sp, #-%d]!", totalFrame);
    genfileinserts("    mov x29, sp");

    /* body */
    inst = *ir->inst;
    while (!inst.end)
    {
        inst = *inst.next;
        genInst(dest, destSz, inst);
    }

    /* epilogue */
    genfileinsertm("%s:", currentEpilogueLabel);
    genfileinsertm("    ldp x29, x30, [sp], #%d", totalFrame);
    genfileinserts("    ret");
}

static void genInst (char *dest, int destSz, IrInst inst)
{
    mccLog("inst %d", inst.op.type);

    switch (inst.op.type)
    {
        case OIT_comment:
            gfis("    ");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_mov:
            gfis("    mov ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_add:
            gfis("    add ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_sub:
            gfis("    sub ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_mul:
            gfis("    mul ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_sdiv:
            gfis("    sdiv ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_and:
            gfis("    and ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_orr:
            gfis("    orr ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_lsr:
            gfis("    lsr ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_cmp:
            gfis("    cmp ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.src);
            gfis("\n");
            break;

        case OIT_cset:
            gfis("    cset ");
            genOp(dest, destSz, inst.dest);
            gfim(", %s", condName(inst.op.cond));
            gfis("\n");
            break;

        case OIT_push:
            gfis("    str ");
            genOp(dest, destSz, inst.dest);
            gfis(", [sp, #-16]!\n");
            break;

        case OIT_pop:
            gfis("    ldr ");
            genOp(dest, destSz, inst.dest);
            gfis(", [sp], #16\n");
            break;

        case OIT_ldr:
            gfis("    ldr ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            if (inst.src.type == OT_reg && inst.src.reg.type == RT_phy)
            {
                gfis("[");
                genOp(dest, destSz, inst.src);
                gfis("]");
            }
            else
            {
                genOp(dest, destSz, inst.src);
            }
            gfis("\n");
            break;

        case OIT_str:
            gfis("    str ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            if (inst.src.type == OT_reg && inst.src.reg.type == RT_phy)
            {
                gfis("[");
                genOp(dest, destSz, inst.src);
                gfis("]");
            }
            else
            {
                genOp(dest, destSz, inst.src);
            }
            gfis("\n");
            break;

        case OIT_call:
            gfis("    bl _");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_ret:
            gfim("    b %s", currentEpilogueLabel);
            gfis("\n");
            break;

        case OIT_jmp:
            gfis("    b ");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_beq:
            gfis("    b.eq ");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_bne:
            gfis("    b.ne ");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_bgt:
            gfis("    b.gt ");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_ble:
            gfis("    b.le ");
            genOp(dest, destSz, inst.dest);
            gfis("\n");
            break;

        case OIT_label:
            genOp(dest, destSz, inst.dest);
            gfis(":\n");
            break;

        case OIT_adr:
            gfis("    adrp ");
            genOp(dest, destSz, inst.dest);
            gfim(", %s@PAGE\n", inst.src.str);
            gfis("    add ");
            genOp(dest, destSz, inst.dest);
            gfis(", ");
            genOp(dest, destSz, inst.dest);
            gfim(", %s@PAGEOFF\n", inst.src.str);
            break;

        default:
            break;
    }
}

void genArm64 (char *dest, int destSz, IrRoutine *ir)
{
    char outFilename[128];
    inpGetFilename(outFilename, sizeof outFilename);

    mccstr(dest, destSz, " \n");
    genfileinserts("// Chaidhat Chaimongkol's MinimalistiC Compiler");
    genfileinsertm("// ARM64 output - %s %s", __TIME__, __DATE__);
    genfileinserts(" ");

    /* string data section */
    {
        int count = getStringLitCount();
        if (count > 0)
        {
            genfileinserts(".section __DATA,__cstring");
            for (int i = 0; i < count; i++)
                genfileinsertm("%s: .asciz \"%s\"", getStringLitLabel(i), getStringLitValue(i));
            genfileinserts(" ");
        }
    }

    /* code section */
    genfileinserts(".section __TEXT,__text");
    genfileinserts(".p2align 2");

    while (!ir->end)
    {
        ir = ir->next;
        genRoutine(dest, destSz, ir);
    }

    genfileinserts(" ");
}
