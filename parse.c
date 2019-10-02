#include "mcc.h"

static bool isDecl ();
static bool isFunc ();

static void readDecl (Tree *parent);
static void readFunc (Tree *oarent);

static void parseVar (Tree *oarent);
static void parseFunc (Tree *parent);
static void parseAssign (Tree *parent);
static void parseStruct (Tree *oarent);
static void parseUnion (Tree *oarent);

static Tree AST;


static bool isDecl ()
{
    bool ret = false;
    lex();
    if (isKw("char") || isKw("int") || isKw("void") || isKw("struct") || isKw("union"))
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
    Tree inst;
    lex();
    if (isKw("char") || isKw("int") || isKw("void"))
        parseVar (parent);
    else if (isKw("struct"))
        parseStruct (parent);
    else if (isKw("union"))
        parseUnion (parent);
    unlex();
}

static void readFunc (Tree *oarent)
{
}



static void parseVar (Tree *oarent)
{
    Tree inst;
    strcpy(inst.id, "placeholder");
    inst.type = IT_Var;
}

static void parseFunc (Tree *parent)
{
}

static void parseAssign (Tree *parent)
{
}

static void parseStruct (Tree *oarent)
{
}

static void parseUnion (Tree *oarent)
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
