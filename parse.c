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
    if (isFunc())
    {
        mccLog("func def");
    }
    else
    {
    }
}

