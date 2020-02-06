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

    for (int i = 0; i < types.childrenSz; i++)
    {
        mccLog("type <%s> <%s> <%d>", peek().id, types.children[i].id, i + 1);
        if (strcmp(peek().id, types.children[i].id) == 0)
            return i + 1;
    }

    mccLog("type invalid");
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

    mccLog("type %d", *type);

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
    funcptr->ast.func.scope = scope;
    
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
        mccLog("scope \"%s\"", peek().id);

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
    inst.ast.type = IT_Var;

    bool findType = true;
    prev(); // for whileloop
    do
    {
        if (findType)
        {
            next(); // expect type
            parseType(&inst.ast.var.varType, &inst.ast.var.isPtr, &inst.ast.var.isStatic);
            if (!inFunc)
                findType = false;
            else if (inst.ast.var.isStatic)
                mccErrC(EC_PARSE_SEM, "unexpected static specifier inside function parameters");
        }

        next(); // expect varname
        strcpy(inst.ast.var.varName, getId());
        strcpy(inst.id, getId());

        next(); // expect "," or ";" or "=" or "["
        
        if (isSep("["))
        {
            inst.ast.var.isArray = true;
            inst.ast.var.isPtr = true;
            inst.ast.var.arrayLength = parseArray();
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

    parseType(&inst->ast.func.retType, &inst->ast.func.isPtr, &inst->ast.func.isStatic);

    next(); // expect funcname
    strcpy(inst->id, getId());
    strcpy(inst->ast.func.funcName, getId());

    next(); // expect "("
    next(); // expect var list (parameters)
    Tree *parameters = crtInst(IT_Scope);
    if (!isSep(")"))
        parseVar(parameters, true); // the elegant variable parsing! :D
    inst->ast.func.parameters = parameters;

    return inst;
}

static void parseAssign (Tree *parent)
{
    static Tree inst;
    inst.ast.type = IT_Assign;

    if (isOp("*")) // must be dereference
    {
        inst.ast.assign.varName = parseDeref();
        strcpy(inst.id, "deref assign");
    }
    else
    {
        Tree *varName = parseId();
        strcpy(inst.id, getId());
        inst.ast.assign.varName = varName;
    }

    next(); // expect "="
    inst.ast.assign.exprsn = parseBinary();


    appendChild(parent, inst);
}

static void parseRet (Tree *parent)
{
    static Tree inst;
    inst.ast.type = IT_Ret;
    strcpy(inst.id, "ret");

    inst.ast.ret.exprsn = parseBinary();

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
    strcpy(inst->ast.call.funcName, getId());
    next(); // expect (

    next(); // expect id or lit or )
    if (!isSep(")"))
        prev();

    Tree *args = crtInst(IT_Scope); // declare on heap, could be recursive
    inst->ast.call.args = args;

    Tree *arg = crtInst(IT_Binary); // declare on heap, could be recursive

    while (!isSep(")"))
    {
        arg = parseBinary();
        if (!isSep(",") && !isSep(")"))
            mccErrC(EC_PARSE_SYN, "expected separator \",\" or end parameters \")\", instead got \"%s\" %s", peek().id,
            inst->ast.call.funcName);
        appendChild(inst->ast.call.args, *arg);
    }
    
    return inst;
}

static Tree *parseLit ()
{
    Tree *inst = crtInst(IT_Lit);
    if (peek().id[0] == '"')
    {
        inst->ast.lit.type = LT_CHAR; // TODO: String handling
        mccErrC(EC_FATAL, "does not do string handling!");
        inst->ast.lit.val.tChar = peek().id[0];
    }
    else
    {
        inst->ast.lit.type = LT_INT;
        int num = mccstrtod(peek().id);
        /**/
        inst->ast.lit.val.tInt = num;
    }
    return inst;
}

static Tree *parseId ()
{
    Tree *inst = crtInst(IT_Id);
    Tree *nested = crtInst(IT_Scope);
    inst->ast.id.nested = nested;
    strcpy(inst->ast.id.varName, peek().id);
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
    inst->ast.binary.stub = false;
    //inst->ast.binary.single = true;

    mccLog("binary \"%s\"", peek().id);
    if (isFunc(false))
        inst->ast.binary.left = parseCall();
    else if (isLit())
        inst->ast.binary.left = parseLit();
    else if (isId())
        inst->ast.binary.left = parseId();
    else if (isPtr())
        inst->ast.binary.left = parsePtr();
    else if (isSep("("))
    {
        inst->ast.binary.left = parseBinary();
        if (!isSep(")"))
            mccErrC(EC_PARSE_SYN, "1 expected operator or endline \")\" in expression");
    }
    else
        mccWarnC(WC_PARSE_SYN, "expected literal or identifier in left expression");

    next(); // expect op or ";"
    if (tokcmpType(T_OP)) // is there an operator?
    {
        //inst->ast.binary.single = false;
        mccLog("op \"%s\"", peek().id);
        inst->ast.binary.op = peek();

        if (isNextExprsn() == -1) // is it the last operator?
        {
            inst->ast.binary.stub = true;
            next(); // expect right side 
            mccLog("op r \"%s\"", peek().id);
            if (isFunc(false))
                inst->ast.binary.right = parseCall();
            else if (isLit())
                inst->ast.binary.right = parseLit();
            else if (isId())
                inst->ast.binary.right = parseId();
            else if (isPtr())
                inst->ast.binary.right = parsePtr();
            else if (isSep("("))
            {
                inst->ast.binary.right = parseBinary();
                if (!isSep(")"))
                    mccErrC(EC_PARSE_SYN, "2 expected operator or endline \")\" in expression");
            }
            else
                mccWarnC(WC_PARSE_SYN, "expected literal or identifier in right expression");
        }
        else if (getPrecedence(inst->ast.binary.op) > isNextExprsn())
        { 
            // e.g. 1 * 2 + 5 should be (1 * 2) + 5
            // e.g. 1 / 2 * 3 + 4 should be ((1 / 2) * 3) + 4 
            mccLog("op p \"%s\"", peek().id);
            Tree *ptrInst = parseBinary();
            Tree *finalLeft = ptrInst;
            mccLog("op p2 \"%s\"", ptrInst->ast.binary.op.id);
            while (!finalLeft->ast.binary.stub) // find the leftmost tree
            {
                finalLeft = finalLeft->ast.binary.left;
            }
            // start off (1 * NULL), (2 + 5)
            // 1. (1 * 2), (NULL + 5)
            // 2. NULL, ((1 * 2) + 5)
            // 3. ((1 * 2) + 5), NULL
            inst->ast.binary.right = finalLeft->ast.binary.left; // 1. right of inst to leftmost
            Tree *tempInst;
            tempInst = inst;
            finalLeft->ast.binary.left = tempInst; // 2. inst inside the leftmost
            inst = ptrInst; // 3. claim the entire left as itself, swap
            prev();
        }
        else
        {
            // e.g. 1 + 2 * 5 should be 1 + (2 * 5)
            // OR 1 + 2 + 3 should be  1 + (2 + 3)
            mccLog("op pp \"%s\"", peek().id);
            inst->ast.binary.right = parseBinary();
            mccLog("op pp2 \"%s\"", inst->ast.binary.right->id);
            prev();
        }
        next(); // expect ";" or next expression
        mccLog("binary \"%s\"", peek().id);
    }
    else
        return inst->ast.binary.left;
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
        inst->ast.deref.exprsn = parseBinary();
        if (!isSep(")"))
            mccErrC(EC_PARSE_SYN, "5 expected operator or endline \")\" in expression");
    }
    else
    {
        if (!isId())
            mccErrC(EC_PARSE_SEM, "expected \"(\" or identifier after \"*\" deref op");
        inst->ast.deref.exprsn = parseId();
    }
    return inst;
}
static Tree *parseRef ()
{
    Tree *inst = crtInst(IT_Ref);
    next(); // expect Id
    if (!isId())
        mccErrC(EC_PARSE_SEM, "expected identifier after \"&\" ref op");
    inst->ast.ref.ptr = parseId();
    return inst;
}



static void parseCond (Tree *parent)
{
    static Tree inst;
    inst.ast.type = IT_Cond;
    strcpy(inst.id, "cond");
    inst.ast.cond.exprsn = parseBinary();

    Tree *scope = crtInst(IT_Scope);
    inst.ast.cond.scope = scope;

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
    inst.ast.type = IT_Ctrl;
    strcpy(inst.id, "ctrl");
    inst.ast.ctrl.exprsn = parseBinary();

    Tree *scope = crtInst(IT_Scope);
    inst.ast.cond.scope = scope;

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
    strcpy(inst->ast.strct.strctName, peek().id);
    crtType(peek().id);
    next(); // expect {
    next();
    inStrctUnin = true;
    while (!isSep("}"))
    {
        mccLog("struct parse lex <%d> \"%s\"", peek().type, peek().id);

        // assume decl
        mccLog("declare");
        Tree *decls = crtInst(IT_Scope);
        inst->ast.strct.decls = decls;
        readDecl(inst->ast.strct.decls);
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
    strcpy(inst->ast.unin.uninName, peek().id);
    crtType(peek().id);
    next(); // expect {
    next();
    inStrctUnin = true;
    while (!isSep("}"))
    {
        mccLog("union parse lex <%d> \"%s\"", peek().type, peek().id);

        // assume decl
        mccLog("declare");
        Tree *decls = crtInst(IT_Scope);
        inst->ast.unin.decls = decls;
        readDecl(inst->ast.unin.decls);
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
            mccLog("next token \"%s\"", token.id);
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
    inst->ast.type = type;
    inst->childrenSz = 0;
    strcpy(inst->id, peek().id);
    return inst;
}



void parse (Tree *ast)
{
    Token t = next();

    if (tokcmpType(T_EOF))
        return;
    mccLog("parse lex <%d> \"%s\"", t.type, t.id);

    if (isFunc(true))
    {
        mccLog("func def");
        readFunc(ast);
    }
    else
    {
        // assume decl
        mccLog("declare");
        readDecl(ast);
    }
}

