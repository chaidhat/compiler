#include "mcc.h"

typedef struct
{
    char dir[128][128];
    Pos pos[128];
    int noFile;
} FileDir;

typedef struct
{
    char name[128];
    char val[128];
} Macro;

static FileDir incFiles;
static Macro macros[128];
static int ifScope = 0;

static Macro NULL_MACRO = {"\0", "0"}; // \0 is null name and macro with value 0 is undefined

static void doInclude (char *inFilepath);

static void addMacro (char *name, char *val);
static void delMarco (char *name);
static Macro *getMacro (char *name);

static void addMacro (char *name, char *val)
{
    for (int i = 0; i < 128; i++)
    {
        if (macros[i].name[0] == '\0')
        {
            strcpy(macros[i].name, name);
            strcpy(macros[i].val, val);
            return;
        }
    }
}

static Macro *getMacro (char *name)
{
    for (int i = 0; i < 128; i++)
    {
        if (strcmp(macros[i].name, name) == 0)
        {
            return &macros[i];
        }
    }
    return &NULL_MACRO;
}

static void doInclude (char *inFilepath)
{
    inpClose();
    inpOpen(inFilepath);
    mccLog("pp reading from %s", inFilepath);
}

char *ppLexeme (char *lexeme)
{
    if (isIgnorePP)
        return lexeme;
    int i = 0;
    if (strcmp(lexeme,"\0") == 0)
        return lexeme;
    while (strcmp(macros[i].name, lexeme) != 0 && macros[i].name[0] != '\0')
        i++;
    if (strcmp(macros[i].name, lexeme) == 0)
        return macros[i].val;
    return lexeme;
}

void ppInit ()
{
    isIgnorePP = false;
    for (int i = 0; i < 128; i++)
    {
        macros[i].name[0] = '\0';
        macros[i].val[0] = '0';
    }
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
    Token macro = lex();
    if (getMacro(macro.id)->name[0] != '\0') // does macro exist
        mccErrC(EC_PP, "redefinition of macro %s", macro.id); // macro exists

    addMacro(macro.id, lex().id);

    mccLog("pp define");
}
void readIf ()
{
    Token macro = lex();
    ifScope++;
    if (strcmp(getMacro(macro.id)->val,lex().id) != 0) // does macro not exist
    { 
        isIgnore = true; // macro does not exists
    }
    mccLog("pp if %s = %s", macro.id, getMacro(macro.id)->val);
}
void readEndif ()
{
    //TODO: no safety for invalid closures
    ifScope--;
    if (ifScope < 0)
        mccErrC(EC_PP, "unexpected keyword \"endif\". \"endif\" does not close any \"if\"");
    isIgnore = false;
    mccLog("pp endif");
}
