#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btcc.h"

// thank you to http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
Token TOKEN_NULL = {0,"NULL",0};

#define T_NULL 0
#define T_LIT 1
#define T_ID 2
#define T_KEY 3
#define T_SEP 4
#define T_OP 5

/* TOKEN TYPES
*  0 - NULL
*  1 - LITERALS
*  2 - IDENTIFIERS
*  3 - KEYWORDS
*  4 - SEPARATORS
*  5 - OPERATORS
*/

// regular expressions
const char* ReKeywords[] =
{
    "define",
    "a",
    "+",
};

const char ReSep[] =
{
    ' ',
    '#',
    ';',
    ':',
    '(',
    ')',
    '<',
    '>',
    '[',
    ']',
    '{',
    '}',
    
    '.',
    ',',

};

const char ReOp[] =
{
    '+',
    '-',
    '*',
    '/',
    '%',

    '&',
    '|',
    '~',
    '^',
    '!',
};

void lex ()
{
    IoLog("lex");

    tokenNo = 0;
    char lexeme[128];
    for (int k = 0; k < 128; k++)
        lexeme[k] = '\0';
    int i = 0;
    int j = 0;
    while (dataBuffer[i] != '\0')
    {
        Token charToken = cmpChar(dataBuffer[i]);
        if (dataBuffer[i] == '\n')
            j--;

        else if (charToken.type != 0)
        {
            if (j > 0)
            {
                lexeme[j] = '\0';

                Token token = getToken(lexeme);
                if (token.type != 0)
                { 
                    token.pos = i;
                    logToken(token);
                }
                else
                {
                    logToken(crtToken(T_LIT,lexeme,i));
                }
            }
            if (charToken.id[0] != ' ')
                logToken(charToken);

            for (int k = 0; k < 128; k++) lexeme[k] = '\0';
            j = -1; 
        }
        else
        {
            lexeme[j] = dataBuffer[i];
        }

        i++;
        j++;
    }
}


Token getToken(char inLexeme[128])
{
    Token t = cmpToken(inLexeme, T_KEY);
    if (t.type != 0)
        return t;
    return TOKEN_NULL;
}

void logToken (Token inToken)
{
    tokens[tokenNo] = inToken; 
    printf("lex %d %d %s\n", tokenNo, inToken.type, inToken.id);
    tokenNo++;
}

Token crtToken (unsigned char type, char id[128], int pos)
{
    Token t = TOKEN_NULL;
    t.type = type;
    strcpy(t.id, id);
    t.pos = pos;
    return t;
} 

Token cmpToken (char inLexeme[128], int cmpTable)
{
    char cmpLexeme[128];
    for (int i = 0; i < sizeof(ReKeywords)/sizeof(ReKeywords[0]); i++)
    {
        switch (cmpTable)
        {
            case T_NULL:
                return TOKEN_NULL;
            case T_KEY:
                strcpy (cmpLexeme, ReKeywords[i]);
                break;
            default:
                break;
        }

        if (strncmp(inLexeme,cmpLexeme,strlen(inLexeme)) == 0)
        {
            return crtToken(cmpTable, cmpLexeme,0);
        }
    }
    return TOKEN_NULL;
}

Token cmpChar (char inChar)
{
    char cmpLexeme[128];
    for (int i = 0; i < sizeof(ReSep)/sizeof(ReSep[0]); i++)
    {
        if (inChar == ReSep[i])
        {
            char inStr[2];
            inStr[0] = inChar;
            inStr[1] = '\0';
            return crtToken(T_SEP,inStr,0);
        }
    }
    return TOKEN_NULL;
}
