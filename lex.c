#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btcc.h"

// thank you to http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html
Token TOKEN_NULL = {0,"NULL",0};
char lexeme[128];
int i = 0, j = 0;

#define T_NULL 0
#define T_LIT 1
#define T_ID 2
#define T_KEY 3
#define T_SEP 4
#define T_OP 5
#define T_COM 6

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
    '=',
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

static Token next ()
{
    i++;
    return crtToken(1,"a",3);
    //TODO: a token memory
}

static void lRec (char in)
{
    lexeme[j] = in;
    j++;
}

static void lClr ()
{
    // a more quicker way of clearing lexeme buffer than lInit()
    while (j > 0)
    {
        lexeme[j] = '\0';
        j--;
    }
}

static void lInit ()
{
    for (int k = 0; k < 128; k++)
        lexeme[k] = '\0';
}

void LexInit ()
{
    lex();
}

void lex ()
{
    IoLog("lex");
    tokenNo = 0;
    lInit();
    while (1)
    {
        char c = inp();
        if (c == '\0')
            return;
        
        Type t = cmpChar(c); 
        printf("%c%d ", c, t);
        switch(t)
        {
            case T_NULL:
                break;
            case T_LIT:
                break;
            case T_ID:
                break;
            case T_KEY:
                break;
            case T_SEP:
                break;
            case T_OP:
                break;
            case T_COM:
                break;
        }

        lRec(c);
    }
    /* while (peek(0) != '\0')
    {
        Token charToken = cmpChar(dataBuffer[i]);
        // comment support
        if (charToken.type == T_COM) 
        {
            if (dataBuffer[i + 1] == '/')
            {
                do
                {i++;}
                while (!(dataBuffer[i] == '\0' || dataBuffer[i] == '\n'));
            }
            else if (dataBuffer[i + 1] == '*')
            {
                do
                {i++;}
                while (!(dataBuffer[i] == '\0' || (dataBuffer[i] == '*' && dataBuffer[i + 1] == '/')));
               
               i += 2;
            }
        }

        // string literal support
        if (charToken.type == T_LIT)
        {
            do
            {
                lexeme[j] = dataBuffer[i];
                i++;
                j++;
                charToken = cmpChar(dataBuffer[i]);
            } while (charToken.type != T_LIT);

        }

        // general lexing
        if (dataBuffer[i] == '\n')
            j--;
        else if (charToken.type != T_NULL)
        {
            if (j > 0)
            {
                lexeme[j] = '\0';

                Token token = getToken(lexeme);
                if (token.type != T_NULL)
                { 
                    token.pos = i;
                    logToken(token);
                }
                else
                {
                    logToken(crtToken(T_ID,lexeme,i));
                }
            }
            if (charToken.id[0] != ' ' && charToken.id[0] != '"')
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
    }*/
}


static Token getToken(char inLexeme[128])
{
    if (inLexeme[0] == '"') // is string literal?
    {
        // get rid of first char
        int i = 0;
        while (inLexeme[i + 1] != '\0')
        {
            inLexeme[i] = inLexeme[i + 1];
            i++;
        }
        inLexeme[i] = '\0';

        return crtToken(T_LIT, inLexeme, 0);
    }
    Token t = cmpToken(inLexeme, T_KEY);
    if (t.type != 0)
        return t;
    return TOKEN_NULL;
}

static void logToken (Token inToken)
{
    tokens[tokenNo] = inToken; 
    printf("lex %d %d %s\n", tokenNo, inToken.type, inToken.id);
    tokenNo++;
}

static Token crtToken (unsigned char type, char id[128], int pos)
{
    Token t = TOKEN_NULL;
    t.type = type;
    strcpy(t.id, id);
    t.pos = pos;
    return t;
} 

static Token cmpToken (char inLexeme[128], int cmpTable)
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

static unsigned char cmpChar (char inChar)
{
    if (inChar == '/')
        return T_COM;
    if (inChar == '"')
        return T_LIT;
    char cmpLexeme[128];
    for (int i = 0; i < sizeof(ReSep)/sizeof(ReSep[0]); i++)
    {
        if (inChar == ReSep[i])
        {
            char inStr[2];
            inStr[0] = inChar;
            inStr[1] = '\0';
            return T_SEP;
        }
    }
    return T_NULL;
}
