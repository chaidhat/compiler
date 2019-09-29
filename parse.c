#include "mcc.h"

static bool isFunc ();

static Tree AST;



static bool isFunc ()
{
    lex();
    if (tokcmpType(T_ID))
    {
        lex();
        if (tokcmpId("("))
        {
            return true;
        }
        unlex();
    }
    unlex();
    return false;
}

void parse (Token t)
{
    mccLog("parse lex %d %s", t.type, t.id);
    Tree a = crtTree("a");
    a.Data.NumberInt.val = 2;
    a.Data.NumberChar.val = 1;
    appendChild(&AST, a);
    printf("%d\n", a.Data.NumberInt.val);
    if (isFunc())
    {
        mccLog("func def");
    }
    else
    {
    }
}

void next()
{
    Token token = ppToken(lex());
    if (tTokcmpType(token, T_NULL))
        return;

    parse(token);

}
