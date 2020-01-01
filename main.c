#include "mcc.h"
#include <windows.h>

static void initConfig ()
{
    strcpy(inFilepath, "$");
    strcpy(outFilepath, "$");
    isChangeFilepath = false;
    mode = 1; // default mode (compiler debug)s
    doBenchmarking = false;
    doParsing = true;
    doAssemble = true;
    doLink = true;
    doWarnings = true;
    doWarningsE = false;
    doDumpAst = false;
}

int main (int argc, char* argv[])
{
    initConfig();
    mccDoArgs(argc, argv);
    mccLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    mccLog("reading from %s", inFilepath);
    mccLog("compiler debugger enabled");
    strcpy(startFilepath, inFilepath);
    inpOpen(inFilepath);
    printf("\n");

    Tree AST;
    AST.childrenSz = 0;

    ppInit();
    do
        parse(&AST);
    while (!tokcmpType(T_EOF));

    if (!doParsing)
        dumpPp();
    else
    {
        inpPop();
        if (doDumpAst)
            dumpAst(&AST);

        Tree IRa;
        IRa.childrenSz = 0;
        genIr(&IRa, &AST);

        Tree IRb;
        IRb.childrenSz = 0;
        map(&IRb, &IRa);

        char sFile[DB_SIZE];
        genX(sFile, sizeof sFile, &IRa);
        
        if (!doAssemble)
            mccExit(0, __LINE__);

        inpClose();
        inpPop();
        inpPush(sFile);
        inpWrite(outFilepath, ".s");
    }
    mccExit(0, __LINE__);
}


