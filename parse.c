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
    next();
    if (isId())
    {
        next();
        if (isSep("("))
        {
            ret = true;
        }
        prev();
    }
    prev();
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
        next(); // expect ;
    }
    if (!isSep(";")) // happy coincidence that union and struct also need ';' as ending
        mccErrC(EC_PARSE_SYN_FAT, "expected endline ';', instead got '%s'", peek().id);
}

static void readFunc (Tree *parent)
{
    Tree *funcptr = parseFunc(parent);

    next();
    if (isSep("{"))
    {
        readScope(funcptr);
    }
    else if (!isSep(";"))
        mccErrC(EC_PARSE_SYN_FAT, "expected endline ';' or scope '{', instead got '%s'", peek().id);
}

static void readScope (Tree *parent)
{
    mccLog("beginscope {");

    while (!isSep("}"))
    {
        next(); // expect next expression
    }

    mccLog("endscope }");
    return;
}



static void parseType (enum LitType *type, bool *isPtr)
{
    if (isKw("char"))
       *type = LT_CHAR;
    if (isKw("int"))
        *type = LT_INT;
    if (isKw("void"))
        *type = LT_VOID;

    next();
    bool initIsPtr = *isPtr;
    *isPtr = isOp("*");
    if (isId())
        prev();
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



static Tree *parseVar (Tree *parent, bool inFunc)
{
    static Tree inst;
    
    bool getType = true;
    prev(); // for whileloop
    do
    {
        if (getType)
        {
            next(); // expect type
            parseType(&inst.Inst.var.varType, &inst.Inst.var.isPtr);
            if (inst.Inst.var.varType == LT_VOID && !inst.Inst.var.isPtr) 
                mccErrC(EC_PARSE_SYN_FAT, "variable has incomplete type \"void\"");
            if (!inFunc)
                getType = false;
        }

        next(); // expect varname
        strcpy(inst.Inst.var.varName, getId());
        strcpy(inst.id, getId());

        if (checkDecl(parent, inst.id))
            appendChild(parent, inst);

        next(); // expect "," or ";"
    } while (isSep(","));
    
    return &inst;
}

static Tree *parseFunc (Tree *parent)
{
    static Tree inst;

    parseType(&inst.Inst.func.retType, &inst.Inst.func.isPtr);

    next(); // expect funcname
    strcpy(inst.id, getId());
    strcpy(inst.Inst.func.funcName, getId());

    if (checkDecl(parent, inst.id))
        appendChild(parent, inst);

    next(); // expect "("
    next(); // expect var list (parameters)
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


void parse ()
{
    Token t = next();

    if (tokcmpType(T_EOF))
        return;
    mccLog("parse lex %d %s", t.type, t.id);

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
}

Token next ()
{
    Token token = ppToken(lex());

    if (tTokcmpType(token, T_NULL)) // is it an EOF that has a lower dir?
        return next();

    return token;
}

Token prev ()
{
    Token token = ppToken(unlex());
    return token;
}
