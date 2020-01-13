#include "mcc.h"
#include <windows.h>

static void configInit ()
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
    configInit();
    mccDoArgs(argc, argv);
    inpOpen(inFilepath);

    mccLog("Written by Chaidhat Chaimongkol. Compiled on %s %s", __DATE__, __TIME__); // -g
    mccLog("-g: Compiler debugger enabled");

    /* compilation front end */

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

        /* compilation back end */

        Tree irDag;
        irDag.childrenSz = 0;
        genIr(&irDag, &AST); // generate IR as DAG from AST

        logTree(&irDag);

        // optimisations, if any, should go here

        IrRoutine *irLinear;
        map(irLinear, &irDag); // convert DAG to linear IR

        char sFile[DB_SIZE];
        genX(sFile, sizeof sFile, irLinear); // generate x86 from linear IR
        
        if (!doAssemble) // -s
            mccExit(0);

        // send to gcc for assembly/linking

        inpClose();
        inpPop();
        inpPush(sFile);
        inpWrite(outFilepath, ".s");
    }
    mccExit(0); // successful exit
}
