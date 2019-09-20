#include "mcc.h"

static void parseDirective ();

static void parseDirective ()
{
    lex();
    if (!tokcmpType(T_KEY))
    {
        mccErrC(EC_PARSE, "unexpected token \"%s\" after \"#\". Expected keyword", tokT.id);
        return;
    }
    else if (tokcmpId("include"))
        readInclude();
    else if (tokcmpId("define"))
        readDefine();
    else if (tokcmpId("ifdef"))
        readIf();
    else if (tokcmpId("endif"))
        readEndif();
    else
        mccErrC(EC_PARSE, "unexpected keyword \"%s\" after \"#\". Expected preprocessor directive", tokT.id);

        
}

void next()
{
    Token inToken = lex();
    if (tokcmpId("#"))
        parseDirective();

    if (isIgnore)
        next();
    else
    {
        printf("parse lex %d %s\n", inToken.type, inToken.id);
    }

    if (!isEOF)
        next();

}

