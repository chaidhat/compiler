#include "mcc.h"

static void setDef ()
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
}

int main (int argc, char* argv[])
{
    setDef();
    mccDoArgs(argc,argv);
    mccLog("Chaidhat Chaimongkol on %s %s", __DATE__, __TIME__);
    mccLog("reading from %s", inFilepath);
    mccLog("writing to %s", outFilepath);
    mccLog("verbose debugger enabled");
    inpOpen(inFilepath);
    printf("\n");

    next();
    //preprocess();
    // do code
    //dataBuffer[2] = 'B';
    inpClose();
    inpOutput(outFilepath);
    mccExit(0, __LINE__);
}


