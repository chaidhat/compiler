#include "mcc.h"
#define a 1

Tree macros;

static void logMacro (char *name, int value);
static int getMacro (char *name);


void readInclude ()
{
}
void readDefine ()
{
    Tree macro = crtTree(lex().id);
    if (deleteChild(&macros, macro.id))
        mccErrC(EC_PP, "redefintion of macro %s", macro.id);

    appendChild(&macros, macro);
    //logMacro(tokT.id, 1);
}
void readIf ()
{
    Tree macro = crtTree(lex().id);
    if (!deleteChild(&macros, macro.id)) // not defined
    { 
        isIgnore = true;
    }
    mccLog("if");
    lex();
}
void readEndif ()
{
    //TODO: no safety for invalid closures
    isIgnore = false;
    mccLog("endif");
}
