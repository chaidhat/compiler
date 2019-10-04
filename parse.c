#include "mcc.h"

static bool isDecl ();
static bool isFunc ();

static void readDecl (Tree *parent);
static void readFunc (Tree *parent);

static void parseType (enum LitType *type, bool *isPtr);
static bool checkDecl (Tree *parent, char *name);

static void parseVar (Tree *parent, bool inFunc);
static void parseFunc (Tree *parent);
static void parseAssign (Tree *parent);
static void parseStruct (Tree *parent);
static void parseUnion (Tree *parent);

static Tree AST;


static bool isDecl ()
{
    bool ret = false;
    lex();
    if (isKw("struct") || isKw("union"))
        ret = true;
    if (isKw("char") || isKw("int") || isKw("void"))
        ret = true;
    unlex();
    return ret;
}

static bool isFunc ()
{
    bool ret = false;
    lex();
    lex();
    if (isId())
    {
        lex();
        if (isSep("("))
        {
            ret = true;
        }
        unlex();
    }
    unlex();
    unlex();
    return ret;
}



static void readDecl (Tree *parent)
{
    lex();
    if (isKw("char") || isKw("int") || isKw("void"))
        parseVar(parent, false);
    else if (isKw("struct"))
        parseStruct (parent);
    else if (isKw("union"))
        parseUnion (parent);
    if (!isSep(";")) // happy coincidence that union and struct also need ';' as ending
        mccErrC(EC_PARSE_SYN, "expected endline ';', instead got '%s'", peek().id);
}

static void readFunc (Tree *parent)
{
    lex();
    parseFunc(parent);

    lex();
    if (isSep("{"))
        mccLog("scope");
    else if (isSep(";"))
        unlex();
    else
        mccErrC(EC_PARSE_SYN, "expected endline ';' or scope '{', instead got '%s'", peek().id);
}



static void parseType (enum LitType *type, bool *isPtr)
{
    if (isKw("char"))
       *type = LT_CHAR;
    if (isKw("int"))
        *type = LT_INT;
    if (isKw("void"))
        *type = LT_VOID;

    lex();
    bool initIsPtr = *isPtr;
    *isPtr = isOp("*");
    if (isId())
        unlex();
    else if (!*isPtr)
        mccErrC(EC_PARSE_SYN, "expected identifier or ptr, instead got \"%s\"", peek().id);
}

static bool checkDecl (Tree *parent, char *name)
{
    if (deleteChild(parent, name)) // does this variable already exist?
    {
        mccErrC(EC_PARSE_SEM, "%s has been declared above", name);
        return false;
    }
    return true;
}



static void parseVar (Tree *parent, bool inFunc)
{
    static Tree inst;

    bool getType = true;
    unlex();
    do
    {
        if (getType)
        {
            lex();
            parseType(&inst.Inst.Var.varType, &inst.Inst.Var.isPtr);
            if (inst.Inst.Var.varType == LT_VOID && !inst.Inst.Var.isPtr) 
                mccErrC(EC_PARSE_SYN, "variable has incomplete type \"void\"");
            if (!inFunc)
                getType = false;
        }

        lex();
        strcpy(inst.Inst.Var.varName, getId());
        strcpy(inst.id, getId());

        checkDecl(parent, inst.id); 
        appendChild(parent, inst);

        lex(); // expect "," or ";"
    } while (isSep(","));
}

static void parseFunc (Tree *parent)
{
    static Tree inst;

    parseType(&inst.Inst.Func.retType, &inst.Inst.Func.isPtr);

    lex();
    strcpy(inst.id, getId());
    strcpy(inst.Inst.Func.funcName, getId());

    checkDecl(parent, inst.id); 
    appendChild(parent, inst);

    lex(); // expect "("
    lex();
    static Tree parameters;
    if (!isSep(")"))
        parseVar(&parameters, true); // the elegant variable parsing! :D
    inst.Inst.Func.parameters = &parameters;
    logTree(inst.Inst.Func.parameters); // debug
}

static void parseAssign (Tree *parent)
{
}

static void parseStruct (Tree *parent)
{
}

static void parseUnion (Tree *parent)
{
}



void parse (Token t)
{
    mccLog("parse lex %d %s", t.type, t.id);
    /*Tree a = crtTree("a");
    a.Data.NumberInt.val = 2;
    a.Data.NumberChar.val = 1;
    appendChild(&AST, a);
    printf("%d\n", a.Data.NumberInt.val);*/
    // only variable and func decl are allowed in global scope
    if (isDecl())
    {
        if (isFunc())
        {
            mccLog("func def");
            readFunc(&AST);
        }
        else
        {
            mccLog("declare");
            readDecl(&AST);
        }
        logTree(&AST);
    }
    /*else
    {
        mccErrC(EC_PARSE_SEM, "expected variable or function declaration");
    }
    */
}

void next()
{
    Token token = ppToken(lex());
    if (tTokcmpType(token, T_NULL))
        return;

    parse(token);

}
