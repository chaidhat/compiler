#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcc.h"

void lex ()
{
    IoLog("lex");

    char token[128];
    int i = 0;
    int j = 0;
    while (dataBuffer[i] != '\0')
    {
        if (checkChar(&dataBuffer[i]) == 1)
        {
            char ch = dataBuffer[i];
            token[j] = '\0';
            if (j > 0)
                printf("lex %s\n",token);
            printf("lex %c\n",ch);
            j = -1; 
        }
        else if (dataBuffer[i] == '\n' || dataBuffer[i] == ' ')
        {
            j--;
        }
        else
        {
            token[j] = dataBuffer[i];
            if (checkToken(token) != -1)
            {
                token[j + 1] = '\0';
                j = -1;
                printf("letx %s\n",token);

            }
        }

        i++;
        j++;
    }
}

int checkChar (char *inChar)
{
    switch (*inChar)
    {
        case '#':
            return 1;
        case ';':
            return 1;
        case '(':
            return 1;
        case ')':
            return 1;
        case '\0':
            return 1;
        default:
            return 0;
    }
}

int checkToken (char inToken[128])
{
    char* args[] =
    {
        "define",
        "undef",
        "include", 
    };
    int argNo = -1;
    for (int i = 0; i < sizeof(args)/sizeof(args[0]); i++)
    {
        if (strncmp(inToken,args[i],strlen(args[i])) == 0)
        {
            return i;
        }
    }
    return -1;
}
