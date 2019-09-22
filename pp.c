#include "mcc.h"

Tree macros;
Tree includeDir;
int ifScope = 0;

void doInclude (char *inFilepath)
{
    inpClose();
    inpOpen(inFilepath);
    mccLog("pp reading from %s", inFilepath);
}

bool prevInclude ()
{
    int noChild = includeDir.noChild;
    if (noChild == 0)
        return false; // end of parsing
    char idir[128];
    if (noChild == 1)
    {
        strcpy(idir, inFilepath);
    }
    else
    {
        strcpy(idir, includeDir.children[noChild - 2].id);
    }
    deleteChild (&includeDir, includeDir.children[noChild - 1].id);
    doInclude(idir);
    return true;
}

void readInclude ()
{
    mccLog("pp include");
    char inFilepath[128];
    strcpy(inFilepath, lex().id);
    appendChild(&includeDir, crtTree(inFilepath));
    doInclude (inFilepath);
}

void readDefine ()
{
    Tree macro = crtTree(lex().id);
    if (deleteChild(&macros, macro.id))
        mccErrC(EC_PP, "redefintion of macro %s", macro.id);

    appendChild(&macros, macro);
    mccLog("pp define");
}
void readIf ()
{
    Tree macro = crtTree(lex().id);
    ifScope++;
    if (!deleteChild(&macros, macro.id)) // not defined
    { 
        isIgnore = true;
    }
    mccLog("pp ifdef");
}
void readEndif ()
{
    //TODO: no safety for invalid closures
    ifScope--;
    if (ifScope < 0)
        mccErrC(EC_PP, "unexpected keyword \"endif\". \"endif\" does not close any \"ifdef\"");
    isIgnore = false;
    mccLog("pp endif");
}
