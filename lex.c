#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

// thank you to http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html

void lex ()
{
    IoLog("lex");

    tokenNo = 0;
    char lexeme[128];
    int i = 0;
    int j = 0;
    while (dataBuffer[i] != '\0')
    {
        Token t = getToken(&dataBuffer[i]);
        if (t.type == 1)
        {
            char ch = dataBuffer[i];
            lexeme[j] = '\0';
            if (j > 0)
                logToken(lexeme);
            lexeme[0] = ch;
            lexeme[1] = '\0';
            if (ch != ' ')
                logToken(lexeme); // token = ch
            j = -1; 
        }
        else if (dataBuffer[i] == '\n')
        {
            j--;
        }
        else
        {
            lexeme[j] = dataBuffer[i];
            if (getToken(lexeme).type == -1)
            {
            }
        }

        i++;
        j++;
    }
}

Token getToken(char *inLexeme[128])
{
    char chars[] =
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
    char* regularExpressions[] =
    {
        "a",
        "+",
    };
    char cmpLexeme[128];
    int argNo = -1;
    for (int i = 0; i < sizeof(chars)/sizeof(chars[0]); i++)
    {
        strcpy (cmpLexeme, regularExpressions[i]);
        if (strncmp(inLexeme,cmpLexeme,strlen(cmpLexeme)) == 0)
        {
            // TODO: is that a memory issue?
            static Token t = {1, regularExpressions[i]};
            return t;
        }
    }
    return NULL;
}

void logToken (Token inToken)
{
    tokens[tokenNo] = inToken; 
    printf("lex %d %s\n", tokenNo, inToken.id);
    tokenNo++;
}
