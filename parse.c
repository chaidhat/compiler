#include "mcc.h"

static void parseDirective ();



static void parseDirective ()
{
    Token inToken = lex();
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
        mccErrC(EC_PARSE, "unexpected keyword \"%s\" after \"#\". Expected preprocessor token", tokT.id);

        
}

void next()
{
    Token inToken = lex();
    if (isEOF)
    {
        if (prevInclude()) 
        {
            isEOF = false;
        }
        return;
    }

    if (tokcmpId("#"))
    {
        parseDirective();
        return;
    }

    if (isIgnore)
        return;

    printf("parse lex %d %s\n", inToken.type, inToken.id);

}

