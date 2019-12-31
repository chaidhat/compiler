#include "mcc.h"

typedef int precedence;
static bool inStrctUnin = false;
static Tree types;
static bool hasInitTypes = false;

static Tree primitiveByte = { .id = "byte" };
static Tree primitiveInt = { .id = "int" };

static enum LitType getType();
static void crtType(char *typeName);
static void parseType (enum LitType *type, bool *isPtr, bool *isStatic);
static bool checkDecl (Tree *parent, char *name);
static precedence getPrecedence (Token op);

static bool isDecl ();
static bool isFunc (bool decl);
static bool isPtr ();
static precedence isNextExprsn ();

static void readDecl (Tree *parent);
static void readFunc (Tree *parent);
static void readScope (Tree *parent);

static void parseVar (Tree *parent, bool inFunc);
static Tree *parseFunc (Tree *parent);
static void parseAssign (Tree *parent);
static void parseRet (Tree *parent);
static Tree *parseArray ();

static Tree *parseCall ();
static Tree *parseLit ();
static Tree *parseId ();
static Tree *parseBinary ();

static void parseStruct (Tree *parent);
static void parseUnion (Tree *parent);

static Tree *parsePtr ();
static Tree *parseDeref ();
static Tree *parseRef ();

static void parseCond (Tree *parent);
static void parseCtrl (Tree *parent);

static Token next ();
static Token prev ();

static Tree *crtInst (enum InstType type);



static enum LitType getType ()
{
    if (!hasInitTypes) // has init?
    {
        // init primitives
        appendChild(&types, primitiveByte);
        appendChild(&types, primitiveInt);

        hasInitTypes = true;
    }

    if (!(tokcmpType(T_KEY) || tokcmpType(T_ID))) // is it a keyword or custom type (id)?
        return LT_INVALID; 

    for (int i = 0; i < types.noChild; i++)
    {
        printf("t %s %s\n", peek().id, types.children[i].id);
        if (strcmp(peek().id, types.children[i].id) == 0)
            return types.noChild - i;
    }

    return LT_INVALID;
}

static void crtType (char *typeName)
{
    static Tree type;
    if (checkDecl(&types, typeName))
    {
        strcpy(type.id, typeName);
        appendChild(&types, type);
    }
}

static void parseType (enum LitType *type, bool *isPtr, bool *isStatic)
{
    *isStatic = false;
    if (isKw("static"))
    {
        *isStatic = true;
        next();
    }

    if (isKw("byte"))
       *type = LT_CHAR;
    else if (isKw("int"))
        *type = LT_INT;
    else if (getType() == LT_INVALID) // type does not exist
        mccErrC(EC_PARSE_SYN, "unexpected declaration type \"%s\"", peek().id);
    else
        *type = getType();

    next();

    if (isKw("static"))
    {
        *isStatic = true;
        next();
    }

    bool initIsPtr = *isPtr;
    *isPtr = isOp("*");
    if (isId())
        prev();
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



static bool isDecl ()
{
    bool ret = false;
    bool p = false;
    if (isKw("static"))
    {
        p = true;
        next();
    }
    if (getType() != LT_INVALID) // type exists
        ret = true;
    if (p)
        prev();
    return ret;
}

static bool isFunc (bool decl)
{
    bool ret = false;
    int a[2];
    *(a+1) = 2;
    if (decl)
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
    if (isOp("*") && decl)
        ret = isFunc(true);
    if (decl)
        prev();
    if (isKw("static") && decl)
    {
        next();
        ret = isFunc(true);
        prev();
    }
    return ret;
}

static bool isPtr ()
{
    bool ret = false;
    if (isOp("*") || isOp("&"))
    {
        next();
        if (isId() || isSep("("))
            ret = true;
        int a = 0;
        int *b = &a;
        prev();
    }
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
    if (isKw("struct"))
        parseStruct(parent);
    else if (isKw("union"))
        parseUnion(parent);
    else
        parseVar(parent, false);
    if (!isSep(";")) // happy coincidence that union and struct also need ';' as ending
        mccErrC(EC_PARSE_SYN, "expected endline ';', instead got '%s'", peek().id);
}

static void readFunc (Tree *parent)
{
    Tree *funcptr = parseFunc(parent);
    Tree *scope = crtInst(IT_Scope);
    funcptr->Inst.func.scope = scope;
    
    next();
    if (isSep("{"))
    {
        readScope(scope);
    }
    else if (!isSep(";"))
        mccErrC(EC_PARSE_SYN, "expected endline ';' or scope '{', instead got '%s'", peek().id);

    if (checkDecl(parent, funcptr->id))
        appendChild(parent, *funcptr);

}

static void readScope (Tree *parent)
{
    mccLog("beginscope {");
    bool ignoreEnd = false;

    next(); // expect next expression
    while (!isSep("}"))
    {
        printf("f %s f\n", peek().id);

        ignoreEnd = true;
        if (isKw("if"))
            parseCond(parent);
        else if (isKw("while"))
            parseCtrl(parent);
        else if (isKw("return"))
            parseRet(parent);
        else if (isFunc(false))
        {
            appendChild(parent, *parseCall());
            next();
            if (!isSep(";"))
                mccErrC(EC_PARSE_SYN, "expected endline ';', instead got '%s'", peek().id);
        }
        else if (isDecl()) 
        {
            parseVar(parent, false);
            if (!isSep(";") && !ignoreEnd)
                mccErrC(EC_PARSE_SYN, "expected endline ';', instead got '%s'", peek().id);
        }
        else if (isId() || isPtr()) // assume assign
        {
            parseAssign(parent);
            if (!isSep(";"))
                mccErrC(EC_PARSE_SYN, "expected endline ',', instead got '%s'", peek().id);
        }
        else
            mccErrC(EC_PARSE_SYN, "expected keyword or identifier statement, instead got \"%s\"", peek().id);


        next();
    }

    mccLog("endscope }");
}



static void parseVar (Tree *parent, bool inFunc)
{
    static Tree inst;
    inst.type = IT_Var;

    bool findType = true;
    prev(); // for whileloop
    do
    {
        if (findType)
        {
            next(); // expect type
            parseType(&inst.Inst.var.varType, &inst.Inst.var.isPtr, &inst.Inst.var.isStatic);
            if (!inFunc)
                findType = false;
            else if (inst.Inst.var.isStatic)
                mccErrC(EC_PARSE_SEM, "unexpected static specifier inside function parameters");
        }

        next(); // expect varname
        strcpy(inst.Inst.var.varName, getId());
        strcpy(inst.id, getId());

        next(); // expect "," or ";" or "=" or "["
        
        if (isSep("["))
        {
            inst.Inst.var.isArray = true;
            inst.Inst.var.isPtr = true;
            inst.Inst.var.arrayLength = parseArray();
        }

        if (checkDecl(parent, inst.id))
            appendChild(parent, inst);

        if (isOp("=")) // assign
        {
            if (!inFunc && !inStrctUnin)
            {
                prev(); // expect varname
                parseAssign(parent);
            }
            else
                mccErrC(EC_PARSE_SEM, "unexpected assignment \"=\" in func param or struct or union");
        }
    } while (isSep(","));
}

static Tree *parseFunc (Tree *parent)
{
    Tree *inst = crtInst(IT_Func);

    parseType(&inst->Inst.func.retType, &inst->Inst.func.isPtr, &inst->Inst.func.isStatic);

    next(); // expect funcname
    strcpy(inst->id, getId());
    strcpy(inst->Inst.func.funcName, getId());

    next(); // expect "("
    next(); // expect var list (parameters)
    Tree *parameters = crtInst(IT_Scope);
    if (!isSep(")"))
        parseVar(parameters, true); // the elegant variable parsing! :D
    inst->Inst.func.parameters = parameters;

    return inst;
}

static void parseAssign (Tree *parent)
{
    static Tree inst;
    inst.type = IT_Assign;

    if (isOp("*")) // must be dereference
    {
        inst.Inst.assign.varName = parseDeref();
    }
    else
    {
        Tree *varName = parseId();
        strcpy(inst.id, getId());
        inst.Inst.assign.varName = varName;
    }

    next(); // expect "="
    inst.Inst.assign.exprsn = parseBinary();


    appendChild(parent, inst);
}

static void parseRet (Tree *parent)
{
    static Tree inst;
    inst.type = IT_Ret;
    strcpy(inst.id, "ret");

    inst.Inst.ret.exprsn = parseBinary();

    if (!isSep(";"))
        mccErrC(EC_PARSE_SYN, "expected endline ';', instead got '%s'", peek().id);
    appendChild(parent, inst);
}

static Tree *parseArray ()
{
    Tree *inst = parseBinary();
    if (!isSep("]"))
        mccErrC(EC_PARSE_SYN, "expected closing bracket ']', instead got '%s'", peek().id);
    next();
    return inst;
}



static Tree *parseCall ()
{
    Tree *inst = crtInst(IT_Call); // declare on heap, could be recursive
    strcpy(inst->Inst.call.funcName, getId());
    next(); // expect (

    next(); // expect id or lit or )
    if (!isSep(")"))
        prev();

    Tree *args = crtInst(IT_Scope); // declare on heap, could be recursive
    inst->Inst.call.args = args;

    Tree *arg = crtInst(IT_Binary); // declare on heap, could be recursive

    while (!isSep(")"))
    {
        arg = parseBinary();
        if (!isSep(",") && !isSep(")"))
            mccErrC(EC_PARSE_SYN, "expected separator \",\" or end parameters \")\", instead got \"%s\" %s", peek().id,
            inst->Inst.call.funcName);
        appendChild(inst->Inst.call.args, *arg);
    }
    
    return inst;
}

static Tree *parseLit ()
{
    Tree *inst = crtInst(IT_Lit);
    if (peek().id[0] == '"')
    {
        inst->Inst.lit.type = LT_CHAR; // TODO: String handling
        mccErrC(EC_FATAL, "does not do string handling!");
        inst->Inst.lit.val.tChar = peek().id[0];
    }
    else
    {
        inst->Inst.lit.type = LT_INT;
        int num = mccstrtod(peek().id);
        /**/
        inst->Inst.lit.val.tInt = num;
    }
    return inst;
}

static Tree *parseId ()
{
    Tree *inst = crtInst(IT_Id);
    Tree *nested = crtInst(IT_Scope);
    inst->Inst.id.nested = nested;
    strcpy(inst->Inst.id.varName, peek().id);
    next(); // check for "."
    if (isSep("."))
    {
        next();
        appendChild(nested, *parseId());

        return inst;
    }
    prev();
    return inst;
}

static Tree *parseBinary ()
{
    next(); // expect left side

    Tree *inst = crtInst(IT_Binary);
    inst->Inst.binary.stub = false;
    //inst->Inst.binary.single = true;

    mccLog("l %s", peek().id);
    if (isFunc(false))
        inst->Inst.binary.left = parseCall();
    else if (isLit())
        inst->Inst.binary.left = parseLit();
    else if (isId())
        inst->Inst.binary.left = parseId();
    else if (isPtr())
        inst->Inst.binary.left = parsePtr();
    else if (isSep("("))
    {
        inst->Inst.binary.left = parseBinary();
        if (!isSep(")"))
            mccErrC(EC_PARSE_SYN, "1 expected operator or endline \")\" in expression");
    }
    else
        mccWarnC(WC_PARSE_SYN, "expected literal or identifier in left expression");

    next(); // expect op or ";"
    if (tokcmpType(T_OP)) // is there an operator?
    {
        //inst->Inst.binary.single = false;
        mccLog("op %s", peek().id);
        inst->Inst.binary.op = peek();

        if (isNextExprsn() == -1) // is it the last operator?
        {
            inst->Inst.binary.stub = true;
            next(); // expect right side 
            mccLog(" %s", peek().id);
            if (isFunc(false))
                inst->Inst.binary.right = parseCall();
            else if (isLit())
                inst->Inst.binary.right = parseLit();
            else if (isId())
                inst->Inst.binary.right = parseId();
            else if (isPtr())
                inst->Inst.binary.right = parsePtr();
            else if (isSep("("))
            {
                inst->Inst.binary.right = parseBinary();
                if (!isSep(")"))
                    mccErrC(EC_PARSE_SYN, "2 expected operator or endline \")\" in expression");
            }
            else
                mccWarnC(WC_PARSE_SYN, "expected literal or identifier in right expression");
        }
        else if (getPrecedence(inst->Inst.binary.op) > isNextExprsn())
        { 
            // e.g. 1 * 2 + 5 should be (1 * 2) + 5
            // e.g. 1 / 2 * 3 + 4 should be ((1 / 2) * 3) + 4 
            mccLog("con %s", peek().id);
            Tree *ptrInst = parseBinary();
            Tree *finalLeft = ptrInst;
            mccLog("f %s", ptrInst->Inst.binary.op.id);
            while (!finalLeft->Inst.binary.stub) // find the leftmost tree
            {
                mccLog("f");
                finalLeft = finalLeft->Inst.binary.left;
            }
            // start off (1 * NULL), (2 + 5)
            // 1. (1 * 2), (NULL + 5)
            // 2. NULL, ((1 * 2) + 5)
            // 3. ((1 * 2) + 5), NULL
            inst->Inst.binary.right = finalLeft->Inst.binary.left; // 1. right of inst to leftmost
            Tree *tempInst;
            tempInst = inst;
            finalLeft->Inst.binary.left = tempInst; // 2. inst inside the leftmost
            inst = ptrInst; // 3. claim the entire left as itself, swap
            prev();
        }
        else
        {
            // e.g. 1 + 2 * 5 should be 1 + (2 * 5)
            // OR 1 + 2 + 3 should be  1 + (2 + 3)
            mccLog("c con %s", peek().id);
            inst->Inst.binary.right = parseBinary();
            mccLog("f %s", inst->Inst.binary.right->id);
            prev();
        }
        next(); // expect ";" or next expression
        mccLog("yyy %s", peek().id);
    }
    else
        return inst->Inst.binary.left;
    //else if (!(isSep(";") || isSep(")") || isSep(",")))
    //    mccWarnC(WC_PARSE_SYN, "expected operator or endline \";\" or \")\" or \",\" in expression");

    return inst;
}



static Tree *parsePtr ()
{
    if (isOp("*"))
        return parseDeref();
    else // expect isOp("&")
        return parseRef();
}
static Tree *parseDeref ()
{
    Tree *inst = crtInst(IT_Deref);
    next();
    if (isSep("("))
    {
        inst->Inst.deref.exprsn = parseBinary();
        if (!isSep(")"))
            mccErrC(EC_PARSE_SYN, "5 expected operator or endline \")\" in expression");
    }
    else
    {
        if (!isId())
            mccErrC(EC_PARSE_SEM, "expected \"(\" or identifier after \"*\" deref op");
        inst->Inst.deref.exprsn = parseId();
    }
    return inst;
}
static Tree *parseRef ()
{
    Tree *inst = crtInst(IT_Ref);
    next(); // expect Id
    if (!isId())
        mccErrC(EC_PARSE_SEM, "expected identifier after \"&\" ref op");
    inst->Inst.ref.ptr = parseId();
    return inst;
}



static void parseCond (Tree *parent)
{
    static Tree inst;
    inst.type = IT_Cond;
    strcpy(inst.id, "cond");
    inst.Inst.cond.exprsn = parseBinary();

    Tree *scope = crtInst(IT_Scope);
    inst.Inst.cond.scope = scope;

    if (isSep("{"))
    {
        readScope(scope);
    }
    else
        mccErrC(EC_PARSE_SEM, "expected scope '{' after exprsn in cond statement", peek().id);

    mccLog("scope if");
    appendChild(parent, inst);
}
static void parseCtrl (Tree *parent)
{
    static Tree inst;
    inst.type = IT_Ctrl;
    strcpy(inst.id, "ctrl");
    inst.Inst.ctrl.exprsn = parseBinary();

    Tree *scope = crtInst(IT_Scope);
    inst.Inst.cond.scope = scope;

    if (isSep("{"))
    {
        readScope(scope);
    }
    else
        mccErrC(EC_PARSE_SEM, "expected scope '{' after exprsn in ctrl statement", peek().id);

    mccLog("scope while");
    appendChild(parent, inst);
}



static void parseStruct (Tree *parent)
{
    next(); // expect strctname

    Tree *inst = crtInst(IT_Strct); // declared on heap because recursion
    strcpy(inst->Inst.strct.strctName, peek().id);
    crtType(peek().id);
    next(); // expect {
    next();
    inStrctUnin = true;
    while (!isSep("}"))
    {
        mccLog("struct parse lex %d %s", peek().type, peek().id);

        // assume decl
        mccLog("declare");
        Tree *decls = crtInst(IT_Scope);
        inst->Inst.strct.decls = decls;
        readDecl(inst->Inst.strct.decls);
        next();
    }
    next();
    inStrctUnin = false;

    if (checkDecl(parent, inst->id))
        appendChild(parent, *inst);
}

static void parseUnion (Tree *parent)
{
    next(); // expect strctname

    Tree *inst = crtInst(IT_Unin); // declared on heap because recursion
    strcpy(inst->Inst.unin.uninName, peek().id);
    crtType(peek().id);
    next(); // expect {
    next();
    inStrctUnin = true;
    while (!isSep("}"))
    {
        mccLog("union parse lex %d %s", peek().type, peek().id);

        // assume decl
        mccLog("declare");
        Tree *decls = crtInst(IT_Scope);
        inst->Inst.unin.decls = decls;
        readDecl(inst->Inst.unin.decls);
        next();
    }
    next();
    inStrctUnin = false;

    if (checkDecl(parent, inst->id))
        appendChild(parent, *inst);
}



static Token next ()
{
    Token token = ppToken(lex());
    if (!doParsing)
    {
        if (tTokcmpType(token, T_NULL) || tTokcmpType(token, T_EOF))
            strcpy(token.id, " ");
        else
            printf("b%sb\n", token.id);
    }

    if (tTokcmpType(token, T_NULL)) // is it an EOF that has a lower dir?
        return next();

    return token;
}

static Token prev ()
{
    Token token = ppToken(unlex());
    return token;
}

static Tree *crtInst (enum InstType type)
{
    Tree *inst = malloc(sizeof(Tree)); // declared on heap
    inst->type = type;
    inst->noChild = 0;
    strcpy(inst->id, peek().id);
    return inst;
}



void parse (Tree *AST)
{
    Token t = next();

    if (tokcmpType(T_EOF))
        return;
    mccLog("parse lex %d %s", t.type, t.id);

    if (isFunc(true))
    {
        mccLog("func def");
        readFunc(AST);
    }
    else
    {
        // assume decl
        mccLog("declare");
        readDecl(AST);
    }
}

