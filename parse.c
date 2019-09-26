#include "mcc.h"
static void parseDirective ();

static Tree AST;
static void parse (Token t);


static void parseDirective ()
{
    PPisIgnoreLex = true;
    Token inToken = lex();

    if (tokcmpId("include"))
        readInclude();
    else if (tokcmpId("define"))
        readDefine();
    else if (tokcmpId("if"))
        readIf();
    else if (tokcmpId("endif"))
        readEndif();
    else
        mccErrC(EC_PARSE, "unexpected keyword \"%s\" after \"#\". Expected preprocessor token", peek().id);
    PPisIgnoreLex = false;

        
}

static void parse (Token t)
{
    printf("parse lex %d %s\n", t.type, t.id);
}

void next()
{
    Token inToken = lex();
    if (tokcmpType(T_EOF))
    {
        if (prevInclude()) 
        {
            resetEOF();
        }
        return;
    }

    if (tokcmpId("#"))
    {
        parseDirective();
        return;
    }

    if (PPisIgnore)
        return;

    if (!doParsing)
    {
        //TODO: dump
        return;
    }

    parse(inToken);

}

