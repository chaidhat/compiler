#include "mcc.h"

static void init ()
{
    ppInit();

    strcpy(inFilepath, "main.mc");
    strcpy(outFilepath, "$");
    mode = 1; // default mode (compiler debug)s
    doBenchmarking = false;
    doRun = false;
    doParsing = true;
    doAssemble = true;
    doLinker = true;
    doWarnings = true;
}

int main (int argc, char* argv[])
{
    init();
    mccDoArgs(argc,argv);
    mccLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    mccLog("reading from %s", inFilepath);
    mccLog("writing to %s", outFilepath);
    mccLog("verbose debugger enabled");
    inpOpen(inFilepath);
    printf("\n");

    /*Tree t;
    t = crtTree("a");
    Tree child = crtTree("zucker");
    Tree child2 = crtTree("gummy");
    Tree child3 = crtTree("bon bon");
    appendChild(&t, child);
    logTree(&t);
    mccLog("1");
    appendChild(&t, child2);
    logTree(&t);
    mccLog("2");
    appendChild(&t, child3);
    logTree(&t);
    mccLog("3");

    appendChild(&child2, child3);
    logTree(&child2);
    mccLog("4");

    deleteChild(&t, "zucker");
    */


    while(!isEOF)
        next();
    /*logTree(&t);
    logTree(&child2);
    */
    //preprocess();
    // do code
    //dataBuffer[2] = 'B';
    inpClose();
    inpOutput(outFilepath);
    mccExit(0, __LINE__);
}


