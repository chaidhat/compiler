#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

// thank you to http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
Token NULL_TOKEN = {0,"NULL",0};

/* TOKEN TYPES
*  0 - NULL
*  1 - KEYWORDS
*  2 - IDENTIFIERS
*  3 - LITERALS
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
    
    '"',
    '\'',
    '.',
    ',',

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
        {
            j--;
        }
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
                    logToken(crtToken(3,lexeme,i));
                }
            }
            if (charToken.id[0] != ' ')
                logToken(charToken);

            for (int k = 0; k < 128; k++)
                lexeme[k] = '\0';
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
    Token t = cmpToken(inLexeme, 0);
    if (t.type != 0)
        return t;
    return NULL_TOKEN;
}

void logToken (Token inToken)
{
    tokens[tokenNo] = inToken; 
    printf("lex %d %d %s\n", tokenNo, inToken.type, inToken.id);
    tokenNo++;
}

Token crtToken (unsigned char type, char id[128], int pos)
{
    Token t = NULL_TOKEN;
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
            case 0:
                strcpy (cmpLexeme, ReKeywords[i]);
                break;
            case 1:
                break;
            default:
                break;
        }

        if (strncmp(inLexeme,cmpLexeme,strlen(inLexeme)) == 0)
        {
            if (i == 0)
                return NULL_TOKEN; // idk why this happens
            return crtToken(1,cmpLexeme,0);
        }
    }
    return NULL_TOKEN;
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
            return crtToken(1,inStr,0);
        }
    }
    return NULL_TOKEN;
}
