#include "mcc.h"

#define genfileinserts(str) if( \
        mccstr(dest, destSz, "%s" str "\n", dest)) \
        { mccErr("gen insert error\n"); mccExit(1, __LINE__); }
#define genfileinsertm(str, ... ) if( \
        mccstr(dest, destSz, "%s" str "\n", dest, __VA_ARGS__)) \
        { mccErr("gen insert error\n"); mccExit(1, __LINE__); }

static void genAllFunc (char *dest, int destSz, Tree *IR);
static void genHeader (char *dest, int destSz, Tree *IR);



static void genAllFunc (char *dest, int destSz, Tree *IR)
{
    genfileinsertm(".globl %s", IR->id);
    for (int i = 0; i < IR->childrenSz; i++)
        genAllFunc(dest, destSz, &IR->children[i]);
}

static void genHeader (char *dest, int destSz, Tree *IR)
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
    genfileinserts(".globl  _main");
    genAllFunc(dest, destSz, IR);
    genfileinserts(" ");
}

void genX (char *dest, int destSz, Tree *IR)
{
    genHeader(dest, destSz, IR);
}
