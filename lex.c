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

typedef unsigned char Type;

//   lexemes
static void lRec (char in);
static void lClr ();
static void lInit ();

static Type getTLexeme (char inLexeme[128]);
static Type getTChar (char inChar);
static void logToken (Token inToken);
static void logChar (char inChar);
static Token crtToken (Type type);

static void readLit ();
static void readCom ();

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
    return crtToken(1);
    //TODO: a token memory
}

static void lRec (char in)
{
    lexeme[j] = in;
    //printf("%c", in);
    j++;
}

static void lClr ()
{
    // a more quicker way of clearing lexeme buffer than lInit()
    while (j >= 0)
    {
        lexeme[j] = '\0';
        j--;
    }
    j++;
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
        if (c == '\n')
            c = ' ';
        
        Type t = getTChar(c); 
        switch(t)
        {
            case T_NULL:
                lRec(c);
                break;
            case T_LIT:
                logToken(crtToken(getTLexeme(lexeme)));
                lClr();
                logChar(c);
                readLit();
                break;
            case T_SEP:
                logToken(crtToken(getTLexeme(lexeme)));
                lClr();
                logChar(c);
                // getTLexeme
                // lIsKey
                // lIsLit
                break;
            case T_OP:
                logToken(crtToken(getTLexeme(lexeme)));
                lClr();
                logChar(c);
                // lIsKey
                // lIsLit
                break;
            case T_COM:
                readCom();
                break;
        }

        //printf("%d ", t);
    }
}


static void logToken (Token inToken)
{
    if (strlen(inToken.id) < 1)
        return;
    tokens[tokenNo] = inToken; 
    printf("lex %d %d %s\n", tokenNo, inToken.type, inToken.id);
    tokenNo++;
}

static void logChar (char inChar)
{
    if (inChar == ' ' || inChar == '"')
        return;
    Token inToken = crtToken(getTChar(inChar));
    strcpy(inToken.id, " ");
    inToken.id[0] = inChar;
    tokens[tokenNo] = inToken; 
    printf("lexc %d %d %s\n", tokenNo, inToken.type, inToken.id);
    tokenNo++;
}


static Token crtToken (Type type)
{
    Token t = TOKEN_NULL;
    t.type = type;
    strcpy(t.id, lexeme);
    t.pos = inpPos;
    return t;
} 

static Type getTLexeme (char inLexeme[128])
{
    // check if it is a keyword or lit
    for (int i = 0; i < sizeof(ReKeywords)/sizeof(ReKeywords[0]); i++)
    {
        if (strncmp(inLexeme,ReKeywords[i],strlen(ReKeywords[i])) == 0)
            return T_KEY;
    }
    if (0/*TODO: check lit*/)
        return T_LIT;
    return T_NULL;
}

static Type getTChar (char inChar)
{
    if (inChar == '/')
        return T_COM;
    if (inChar == '"')
        return T_LIT;
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
    for (int i = 0; i < sizeof(ReOp)/sizeof(ReOp[0]); i++)
    {
        if (inChar == ReOp[i])
        {
            char inStr[2];
            inStr[0] = inChar;
            inStr[1] = '\0';
            return T_OP;
        }
    }
    return T_NULL;
}

static void readLit ()
{
    while (!(getTChar(inp()) == T_LIT) && !inpCT('\0'))
        lRec(inpT);
    if (inpCT('\0'))
    {
        IoErr("lex: unterminated string");
        IoExit(1, __LINE__);
    }
    logToken(crtToken(T_LIT));
    lClr();
}

static void readCom ()
{
    if (inpN == '/')
    {
        while (!inpCT('\n') && !inpCT('\0'))
            inp();
    }
    if (inpN == '*')
    {
        while (!(inpCT('*') && inpCN('/')) && !inpCT('\0'))
            inp();
    }
}
