#include "mcc.h"

static void readDirective ()
{
    lex();
    if (!tokcmpType(T_KEY))
    {
        mccErrC(EC_PARSE, "unexpected token \"%s\" after \"#\". Expected preprocessor directive", tokT.id);
        return;
    }
    if (tokcmpId("define"))
    {
        printf("define %s\n", lex().id);
        return;
    }
    else if (tokcmpId("if"))
    {
        printf("if %s\n", lex().id);
        return;
    }
    else
    {
        mccErrC(EC_PARSE, "unexpected keyword \"%s\" after \"#\"", tokT.id);
    }

        
}

void next()
{
    Token inToken = lex();
    printf("pp lex %d %s\n", inToken.type, inToken.id);
    if (tokcmpId("#"))
        readDirective();

    if (tokcmpId("#"))
        printf("directive\n");

    if (!isEOF)
        next();
}

