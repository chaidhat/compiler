#include "mcc.h"

static void init ()
{
    strcpy(inFilepath, "main.mc");
    strcpy(outFilepath, "$");
    mode = 1; // default mode (compiler debug)s
    doBenchmarking = false;
    doRun = false;
    doParsing = true;
    doAssemble = true;
    doLinker = true;
    doWarnings = true;
    doWarningsE = false;
    doDumpAst = false;
    doDumpSta = false;
}

int main (int argc, char* argv[])
{
    init();
    mccDoArgs(argc,argv);
    mccLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    mccLog("reading from %s", inFilepath);
    mccLog("writing to %s", outFilepath);
    mccLog("verbose debugger enabled");
    strcpy(startFilepath, inFilepath);
    inpOpen(inFilepath);
    printf("\n");

    ppInit();
    do
        parse();
    while (!tokcmpType(T_EOF));

    if (doDumpAst)
        dumpAst(&AST);
    inpPush("aa");
    inpClose();
    inpWrite(outFilepath);
    mccExit(0, __LINE__);
}


