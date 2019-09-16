#include "mcc.h"

static void parseDirective ();

static void parseDirective ()
{
    lex();
    if (!tokcmpType(T_KEY))
    {
        mccErrC(EC_PARSE, "unexpected token \"%s\" after \"#\". Expected preprocessor directive", tokT.id);
        return;
    }
    else if (tokcmpId("define"))
        readDefine();
    else if (tokcmpId("if"))
        readIf();
    else
        mccErrC(EC_PARSE, "unexpected keyword \"%s\" after \"#\"", tokT.id);

        
}

void next()
{
    Token inToken = lex();
    printf("parse lex %d %s\n", inToken.type, inToken.id);
    if (tokcmpId("#"))
        parseDirective();

    if (!isEOF)
        next();
}

