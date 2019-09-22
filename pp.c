#include "mcc.h"

Tree macros;
typedef struct
{
    char dir[128][128];
    Pos pos[128];
    int noFile;
} Files;

static Files incFiles;
static int ifScope = 0;
static void doInclude (char *inFilepath);

static void doInclude (char *inFilepath)
{
    inpClose();
    inpOpen(inFilepath);
    mccLog("pp reading from %s", inFilepath);
}

void ppInit ()
{
    incFiles.noFile = 0;
}

bool prevInclude ()
{
    int *nf = &incFiles.noFile;
    if (*nf == 0)
        return false; // end of parsing
    char idir[128];
    if (*nf == 1)
    {
        strcpy(idir, inFilepath);
    }
    else
    {
        strcpy(idir, incFiles.dir[*nf - 2]);
    }
    *nf = *nf - 1;

    doInclude(idir);
    inpGoto(incFiles.pos[incFiles.noFile]);
    return true;
}

void readInclude ()
{
    char inFilepath[128];
    strcpy(inFilepath, lex().id);

    incFiles.noFile++;

    strcpy(incFiles.dir[incFiles.noFile - 1], inFilepath);
    incFiles.pos[incFiles.noFile - 1] = inpPos;

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
