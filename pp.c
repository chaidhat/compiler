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

static const Token TOKEN_NULL = {T_NULL,"NULL",0};
static const Token TOKEN_EOF = {T_EOF,"EOF",0};
static Macro MACRO_NULL = {"\0", "0"}; // \0 is null name and macro with value 0 is undefined

static void doInclude (char *inFilepath);
static bool prevInclude ();

static void init ();
static bool isInit = false;

static void addMacro (char *name, char *val);
static void delMarco (char *name);
static Macro *getMacro (char *name);

static void readInclude ();
static void readDefine ();
static void readIf ();
static void readEndif ();

static bool PPisIgnore;
static bool PPisIgnoreLex;

static void parseDirective ();

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
    return &MACRO_NULL;
}

static void doInclude (char *inFilepath)
{
    mccLog("pp reading from %s", inFilepath);
    inpClose();
    inpOpen(inFilepath);
}

static bool prevInclude ()
{
    int *nf = &incFiles.noFile;
    if (*nf == 0)
        return false; // end of parsing
    char idir[128];
    if (*nf == 1)
    {
        strcpy(idir, startFilepath);
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

static void init ()
{
    if (!isInit)
    {
        mccLog("initPP");
        PPisIgnoreLex = false;
        for (int i = 0; i < 128; i++)
        {
            macros[i].name[0] = '\0';
            macros[i].val[0] = '0';
        }
        incFiles.noFile = 0;
        isInit = true;
    }
}

static void readInclude ()
{
    char inFilepath[128];
    strcpy(inFilepath, lex().id);

    incFiles.noFile++;

    strcpy(incFiles.dir[incFiles.noFile - 1], inFilepath);
    incFiles.pos[incFiles.noFile - 1] = inpPos;

    doInclude (inFilepath);
}

static void readDefine ()
{
    Token macro = lex();
    if (getMacro(macro.id)->name[0] != '\0') // does macro exist
        mccErrC(EC_PP, "redefinition of macro %s", macro.id); // macro exists

    addMacro(macro.id, lex().id);

    mccLog("pp define");
}
static void readIf ()
{
    Token macro = lex();
    ifScope++;
    if (strcmp(getMacro(macro.id)->val,lex().id) != 0) // does macro not exist
    { 
        PPisIgnore = true; // macro does not exists
    }
    mccLog("pp if %s = %s", macro.id, getMacro(macro.id)->val);
}
static void readEndif ()
{
    //TODO: no safety for invalid closures
    ifScope--;
    if (ifScope < 0)
        mccErrC(EC_PP, "unexpected keyword \"endif\". \"endif\" does not close any \"if\"");
    PPisIgnore = false;
    mccLog("pp endif");
}

static void parseDirective ()
{
    PPisIgnoreLex = true;
    Token inToken = lex();

    if (tokcmpId("include"))
        readInclude();
    else if (tokcmpId("define"))
        readDefine();
    else if (tokcmpId("if"))
        readIf();
    else if (tokcmpId("endif"))
        readEndif();
    else
        mccErrC(EC_PP, "unexpected keyword \"%s\" after \"#\". Expected preprocessor token", peek().id);
    PPisIgnoreLex = false;

        
}

void predefineMacro (char *name, char *val)
{
    addMacro(name, val);
}

void predefineInclude (char *dir)
{
    init();
    mccLog(dir);

    incFiles.noFile++;


    strcpy(incFiles.dir[incFiles.noFile - 1], dir);
    incFiles.pos[incFiles.noFile - 1] = inpPos;

}

char *ppLexeme (char *lexeme)
{
    if (PPisIgnoreLex) // PPisIgnoreLex is on when parsing preprocessing directives
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

Token ppToken (Token token)
{
    if (tTokcmpType(token, T_EOF))
    {
        if (prevInclude()) 
        {
            resetEOF();
            return TOKEN_NULL;
        }
        return TOKEN_EOF;
    }

    if (tTokcmpId(token, "#"))
    {
        parseDirective();
        return TOKEN_NULL;
    }

    if (PPisIgnore)
        return TOKEN_NULL;

    if (!doParsing)
    {
        inpPush(peek().id);
        inpPush(" ");
        return TOKEN_NULL;
    }

    return token;
}

void ppInit ()
{
    init();
    if (incFiles.noFile > 0)
    {
        doInclude(incFiles.dir[incFiles.noFile - 1]);
    }
}

