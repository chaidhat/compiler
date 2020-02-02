#include "mcc.h"
#include <windows.h>

static void configInit ()
{
    mccstr(inFilepath, 128, "$");
    mccstr(outFilepath, 128, "$");
    isChangeFilepath = false;
    mode = 1; // default mode (compiler debug)s
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

    Tree ast;
    ast.childrenSz = 0;
    do
        parse(&ast); // preprocess, lex, parse the input file
    while (!tokcmpType(T_EOF));

    if (!doParsing) // -E
        dumpPp();
    else
    {
        inpPop();

        if (doDumpAst) // -fd
            dumpAst(&ast);

        /* compilation back end */

        IrRoutine *ir = crtRoutine("");
        genIr(ir, &ast); // generate IR from AST

        ir = memalloc(ir); // assign registers to IR

        char outFile[DB_SIZE];
        genX(outFile, sizeof outFile, ir); // generate x86 from IR
        
        if (!doAssemble) // -S
        {
            inpClose();
            inpPop();
            inpPush(outFile);
            inpWrite(outFilepath, ".s");
        }
        else
        {
            // send to gcc for assembly/linking
        }

    }
    mccExit(0); // successful exit
}
