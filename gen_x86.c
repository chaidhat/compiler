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

static void genRoutine (char *dest, int destSz, IrRoutine *ir);
static void genInst (char *dest, int destSz, IrInst inst);


static void genRoutine (char *dest, int destSz, IrRoutine *ir)
{
    IrInst inst;
    inst = *ir->inst; // first inst is stub
    while (!inst.end)
    {
        inst = *inst.next; // first inst is stub
        genInst(dest, destSz, inst);
    }
}

static void genInst (char *dest, int destSz, IrInst inst)
{
    mccLog("inst");
    if (inst.op.type == OIT_push)
    {
        mccLog("inst push");
        gfis("push");
        if (inst.op.size == OMT_long)
            gfis("l ");
        if (inst.dest.type == OT_str_lit)
            gfim("%s", inst.dest.str);
        if (inst.dest.type == OT_num_lit)
            gfim("%d", inst.dest.num);
        gfis("\n");
    }
    /*
    gfis(" ");
    if (inst.src.type == OT_str_lit)
        gfim("%s", inst.src.str);
    if (inst.src.type == OT_num_lit)
        gfim("%d", inst.src.num);
        */
}

void genX (char *dest, int destSz, IrRoutine *ir)
{
    char outFilename[128];
    inpGetFilename(outFilename, sizeof outFilename);

    mccstr(dest, destSz, " \n");
    genfileinserts("# Chaidhat Chaimongkol's #");
    genfileinserts("# MinimalistiC Compiler  #");
    genfileinsertm("# on %s %s #", __TIME__, __DATE__);
    genfileinserts(" ");
    genfileinsertm(".file   \"%s%s\"", outFilename, isChangeFilepath ? " " : ".mc");
    genfileinserts(" ");
    genfileinserts("# global declarations");
    genfileinserts(".data");
    genfileinserts(" ");
    genfileinserts("# global function declarations");
    genfileinserts(".text");

    genRoutine(dest, destSz, ir);
    while (!ir->end)
    {
        ir = ir->next;
        genRoutine(dest, destSz, ir);
    }
        mccLog("ndinst");

    genfileinserts(" ");
}
