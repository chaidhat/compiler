#include "btcc.h"

char macros[128][128];
int macroNo = 0;

static void readDirective ()
{
    next();
    while (!peekType(T_EOF))
    {
        if (peekType(T_SEP))
        {
            if (peek("#"))
            {
                printf("#\n");
                next();
                if (peek("include"))
                {
                }
                if (peek("define"))
                    strcpy(macros[macroNo++], next().id);
                // #if, #ifdef, #ifndef, #else, #elif and #endi
                if (peek("if"))
                {
                }
                if (peek("ifdef"))
                {
                }
                if (peek("ifndef"))
                {
                }
                if (peek("else"))
                {
                }
                if (peek("elif"))
                {
                }
                if (peek("endi"))
                {
                }
                if (peek("error"))
                {
                }
            }

        }
        next();

    }
}

void readNext()
{
    bool lstop = false;
    while (!lstop)
    {
        Token inToken = lex(&lstop);
        printf("cpp lex %d %d %s\n", tokenNo, inToken.type, inToken.id);
    }
}

