#include "mcc.h"

static bool isDecl ();
static bool isFunc ();

static void readDecl (Tree *parent);
static void readFunc (Tree *parent);
static void readScope (Tree *parent);

static void parseType (enum LitType *type, bool *isPtr);
static bool checkDecl (Tree *parent, char *name);

static Tree *parseVar (Tree *parent, bool inFunc);
static Tree *parseFunc (Tree *parent);
static void parseAssign (Tree *parent);
static void parseStruct (Tree *parent);
static void parseUnion (Tree *parent);

static Tree AST;


static bool isFunc ()
{
    bool ret = false;
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
    return ret;
}



static void readDecl (Tree *parent)
{
    if (isKw("char") || isKw("int") || isKw("void"))
        parseVar(parent, false);
    else if (isKw("struct"))
        parseStruct(parent);
    else if (isKw("union"))
        parseUnion(parent);
    else
    {
        mccErrC(EC_PARSE_SYN, "unknown type for declaration \"%s\"", peek().id);
        lex();
    }
    if (!isSep(";")) // happy coincidence that union and struct also need ';' as ending
        mccErrC(EC_PARSE_SYN_FAT, "expected endline ';', instead got '%s'", peek().id);
}

static void readFunc (Tree *parent)
{
    Tree *funcptr = parseFunc(parent);

    lex();
    if (isSep("{"))
    {
        lex();
        readScope(funcptr);
    }
    else if (!isSep(";"))
        mccErrC(EC_PARSE_SYN_FAT, "expected endline ';' or scope '{', instead got '%s'", peek().id);
}

static void readScope (Tree *parent)
{
    if (isSep("}"))
    {
        lex();
        return;
    }
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
        mccErrC(EC_PARSE_SYN_FAT, "expected identifier or ptr, instead got \"%s\"", peek().id);
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

static void parseScope (Tree *parent)
{
}




static Tree *parseVar (Tree *parent, bool inFunc)
{
    static Tree inst;
    
    bool getType = true;
    unlex();
    do
    {
        if (getType)
        {
            lex();
            parseType(&inst.Inst.var.varType, &inst.Inst.var.isPtr);
            if (inst.Inst.var.varType == LT_VOID && !inst.Inst.var.isPtr) 
                mccErrC(EC_PARSE_SYN_FAT, "variable has incomplete type \"void\"");
            if (!inFunc)
                getType = false;
        }

        lex();
        strcpy(inst.Inst.var.varName, getId());
        strcpy(inst.id, getId());

        if (checkDecl(parent, inst.id))
            appendChild(parent, inst);

        lex(); // expect "," or ";"
    } while (isSep(","));
    
    return &inst;
}

static Tree *parseFunc (Tree *parent)
{
    static Tree inst;

    parseType(&inst.Inst.func.retType, &inst.Inst.func.isPtr);

    lex();
    strcpy(inst.id, getId());
    strcpy(inst.Inst.func.funcName, getId());

    if (checkDecl(parent, inst.id))
        appendChild(parent, inst);

    lex(); // expect "("
    lex();
    static Tree parameters;
    if (!isSep(")"))
        parseVar(&parameters, true); // the elegant variable parsing! :D
    inst.Inst.func.parameters = &parameters;

    return &inst;
    //logTree(inst.Inst.func.parameters); // debug
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
    if (isFunc())
    {
        mccLog("func def");
        readFunc(&AST);
    }
    else
    {
        // assume decl
        mccLog("declare");
        readDecl(&AST);
    }
    //logTree(&AST);
    /*else
    {
        // or assume type "int"
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
