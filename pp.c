#include "mcc.h"

char macros[128][128];
int macroNo = 0;

static void readDirective ()
{
    lex();
    if (tokcmpId("define"))
        printf("define %s\n", lex().id);
}

void next()
{
    while (!isEOF)
    {
        Token inToken = lex();
        printf("pp lex %d %s\n", inToken.type, inToken.id);
        if (tokcmpId("#"))
            readDirective();

        if (tokcmpId("#"))
            printf("directive\n");
    }
}

