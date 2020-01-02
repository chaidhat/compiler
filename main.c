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

    /* front end */

    ppInit();

    Tree AST;
    AST.childrenSz = 0;
    do
        parse(&AST); // preprocess, lex, parse the input file
    while (!tokcmpType(T_EOF));

    if (!doParsing) // -E
        dumpPp();
    else
    {
        inpPop();

        if (doDumpAst) // -fd
            dumpAst(&AST);

        /* back end */

        Tree irDag;
        irDag.childrenSz = 0;
        genIr(&irDag, &AST); // generate IR as DAG from AST

        // optimisations, if any, should go here

        IrRoutine *irLinear;
        map(irLinear, &irDag); // convert DAG to linear IR

        char sFile[DB_SIZE];
        genX(sFile, sizeof sFile, irLinear); // generate x86 from linear IR
        
        if (!doAssemble) // -s
            mccExit(0, __LINE__);

        // send to gcc for assembly/linking

        inpClose();
        inpPop();
        inpPush(sFile);
        inpWrite(outFilepath, ".s");
    }
    mccExit(0, __LINE__);
}


