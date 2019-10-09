#include "mcc.h"

typedef int precedence;

static bool isFunc ();
static precedence isNextExprsn ();

static void readDecl (Tree *parent);
static void readFunc (Tree *parent);
static void readScope (Tree *parent);

static void parseType (enum LitType *type, bool *isPtr);
static bool checkDecl (Tree *parent, char *name);
static precedence getPrecedence (Token op);

static Tree *parseVar (Tree *parent, bool inFunc);
static Tree *parseFunc (Tree *parent);
static Tree *parseAssign (Tree *parent);

static Tree *parseLit ();
static Tree *parseId ();
static Tree *parseBinary ();

static void parseStruct (Tree *parent);
static void parseUnion (Tree *parent);

static Token next ();
static Token prev ();


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
    if (isOp("*"))
        ret = isFunc();
    prev();
    return ret;
}

static precedence isNextExprsn ()
{
    Token op;
    bool ptr = false;
    int i = 0;
    int scope = 0;
    while (!isSep(";"))
    {
        next();
        i++;
        if (isSep("("))
            scope++;
        else if (isSep(")"))
            scope--;
        else if (tokcmpType(T_OP) && scope == 0) // is an op on the same scope
        {
            ptr = true;
            op = peek();
            break;
        }
    }
    for (int j = 0; j < i; j++)
        prev();
    if (ptr)
    {
        return getPrecedence(op);
    }
    return -1;
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

static int getPrecedence (Token op)
{
    // the higher, the more important
    if (tTokcmpId(op, "-"))
        return 0;
    if (tTokcmpId(op, "+"))
        return 1;
    if (tTokcmpId(op, "*"))
        return 2;
    if (tTokcmpId(op, "/"))
        return 3;
    if (tTokcmpId(op, "==") || tTokcmpId(op, "!=") || tTokcmpId(op, "&&") || tTokcmpId(op, "||"))
        return 4;
    return -1;
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

        next(); // expect "," or ";" or "="

        if (isOp("=")) // assign
        {
            prev(); // expect varname
            Tree *instAssign = parseAssign(parent);
        }
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
}

static Tree *parseAssign (Tree *parent)
{
    static Tree inst;
    inst.type = IT_Assign;

    strcpy(inst.id, getId());
    strcpy(inst.Inst.assign.varName, getId());

    next(); // expect "="
    parseBinary(inst.Inst.assign.exprsn);

    return &inst;
}



static Tree *parseLit ()
{
    static Tree inst;
    return &inst;
}

static Tree *parseId ()
{
    static Tree inst;
    return &inst;
}

static Tree *parseBinary ()
{
    static Tree inst;
    inst.type = IT_Binary;
    inst.Inst.binary.stub = false;

    next(); // expect left side
    strcpy(inst.id, peek().id);
    mccErr("l %s", peek().id);
    if (isLit())
        inst.Inst.binary.left = parseLit();
    else if (isId())
        inst.Inst.binary.left = parseId();
    else if (isSep("("))
    {
        inst.Inst.binary.left = parseBinary();
    }
    else
        mccErrC(EC_PARSE_SYN, "expected literal or identifier in left expression");

    next(); // expect op or ";"
    if (tokcmpType(T_OP)) // is there an operator?
    {
        mccErr("op %s", peek().id);
        inst.Inst.binary.op = peek();

        if (isNextExprsn() == -1) // is it the last operator?
        {
            inst.Inst.binary.stub = true;
            next(); // expect right side 
            mccErr("r %s", peek().id);
            if (isLit())
                inst.Inst.binary.right = parseLit();
            else if (isId())
                inst.Inst.binary.right = parseId();
            else if (isSep("("))
            {
                inst.Inst.binary.right = parseBinary();
            }
            else
                mccErrC(EC_PARSE_SYN, "expected literal or identifier in right expression");
        }
        else if (getPrecedence(inst.Inst.binary.op) > isNextExprsn())
        { 
            // e.g. 1 * 2 + 5 should be (1 * 2) + 5
            // e.g. 1 / 2 * 3 + 4 should be ((1 / 2) * 3) + 4 
            mccErr("con %s", peek().id);
            Tree *ptrInst = parseBinary();
            Tree *finalLeft = ptrInst;
            mccErr("f %s", ptrInst->Inst.binary.op.id);
            while (!finalLeft->Inst.binary.stub) // find the leftmost tree
            {
                mccErr("f");
                finalLeft = finalLeft->Inst.binary.left;
            }
            // start off (1 * NULL), (2 + 5)
            // 1. (1 * 2), (NULL + 5)
            // 2. NULL, ((1 * 2) + 5)
            // 3. ((1 * 2) + 5), NULL
            inst.Inst.binary.right = finalLeft->Inst.binary.left; // 1. right of inst to leftmost
            static Tree tempInst;
            tempInst = inst;
            finalLeft->Inst.binary.left = &tempInst; // 2. inst inside the leftmost
            inst = *ptrInst; // 3. claim the entire left as itself, swap
            prev();
        }
        else
        {
            // e.g. 1 + 2 * 5 should be 1 + (2 * 5)
            // OR 1 + 2 + 3 should be  1 + (2 + 3)
            mccErr("c con %s", peek().id);
            inst.Inst.binary.right = parseBinary();
            mccErr("f %s", inst.Inst.binary.right->id);
            prev();
        }
        next(); // expect ";" or next expression
        mccErr("yyy %s", peek().id);
    }
    else if (isSep(")"))
    {
        
    }
    else if (!isSep(";"))
        mccErrC(EC_PARSE_SYN, "expected operator or endline \";\" or \")\" in expression");

    return &inst;
}



static void parseStruct (Tree *parent)
{
}

static void parseUnion (Tree *parent)
{
}



static Token next ()
{
    Token token = ppToken(lex());

    if (tTokcmpType(token, T_NULL)) // is it an EOF that has a lower dir?
        return next();

    return token;
}

static Token prev ()
{
    Token token = ppToken(unlex());
    return token;
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

