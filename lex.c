#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

void lex ()
{
    IoLog("lex");

    tokenNo = 0;
    char token[128];
    int i = 0;
    int j = 0;
    while (dataBuffer[i] != '\0')
    {
        if (checkOperator(&dataBuffer[i]) == 1)
        {
            char ch = dataBuffer[i];
            token[j] = '\0';
            if (j > 0)
                logToken(token);
            token[0] = ch;
            token[1] = '\0';
            if (ch != ' ')
                logToken(token); // token = ch
            j = -1; 
        }
        else if (dataBuffer[i] == '\n')
        {
            j--;
        }
        else
        {
            token[j] = dataBuffer[i];
        }

        i++;
        j++;
    }
}

int checkOperator (char *inToken)
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
    int argNo = -1;
    for (int i = 0; i < sizeof(chars)/sizeof(chars[0]); i++)
    {
        if (*inToken == chars[i])
            return 1;
    }
    return 0;
}

void logToken (char inToken[128])
{
    strcpy(tokens[tokenNo], inToken);
    printf("lex %d %s\n", tokenNo, inToken);
    tokenNo++;
}
