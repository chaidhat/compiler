#include <ctype.h>
#include "btcc.h"

Token TOKEN_NULL = {0,"NULL",0};
char lexeme[128];
int i = 0, j = 0;

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

static void lRec (char in)
{
    lexeme[j] = in;
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



static Type getTLexeme (char inLexeme[128])
{
    // check if it is a keyword or lit
    for (int i = 0; i < sizeof(ReKeywords)/sizeof(ReKeywords[0]); i++)
    {
        if (strncmp(inLexeme,ReKeywords[i],strlen(ReKeywords[i])) == 0)
            return T_KEY;
    }
    for (int i = 0; i < 128; i++)
    {
        if (!isdigit(inLexeme[i]) && inLexeme[i] != '\0')
            return T_ID;
    }
    return T_LIT;
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
    //TODO: no error catch
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
    lClr();
    if (inChar == ' ' || inChar == '"')
        return;
    Token inToken = crtToken(getTChar(inChar));
    strcpy(inToken.id, " ");
    inToken.id[0] = inChar;
    tokens[tokenNo] = inToken; 
    printf("lex %d %d %s\n", tokenNo, inToken.type, inToken.id);
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

static void readLit ()
{
    Pos p = inpPos;
    while (!(getTChar(inp()) == T_LIT) && !inpCT('\0') && !inpCT('\n'))
        lRec(inpT);
    if (inpCT('\0') || inpCT('\n'))
    {
        inpPos = p;
        btccErrC(LEX, "unterminated '\"'");
    }
    logToken(crtToken(T_LIT));
    lClr();
}

static void readCom ()
{
    Pos p = inpPos;
    if (inpN == '/')
    {
        while (!inpCT('\n') && !inpCT('\0'))
            inp();
    }
    if (inpN == '*')
    {
        while (!(inpCT('*') && inpCN('/')) && !inpCT('\0'))
        {
            inp();
            if (inpCT('/') && inpCN('*'))
            {     
                inpPos = p;
                btccErrC(LEX, "overlapping '/*'. Did not expect '/*'");
            }
        }
        if (inpCT('\0'))
        {
            inpPos = p;
            btccErrC(LEX, "unterminated '/*'. Expecting '*/' to close '/*'");
        }
    }
}

Token next ()
{
    tokenNo++;
    return tokens[tokenNo - 1];
}

bool peek (const char* expect)
{
    if (strncmp(tokens[tokenNo - 1].id,expect,strlen(expect)) == 0)
        return true;
    return false;
}

bool peekType (Type expect)
{
    if (tokens[tokenNo - 1].type == expect)
        return true;
    return false;
}
void lexAll ()
{
    tokenNo = 0;
    lInit();
    bool lstop = false;
    while (!lstop)
        lex(&lstop);
}

Token lex (bool *stop)
{
    bool cont = true;
    while (cont)
    {
        cont = false;
        if (getTChar(inpT) != T_NULL)
        {
            logChar(inpT);

            // sets inpT to T_NULL so this does not infinite loop
            inpT = 'a'; 
        }
        else
        {
            char c = inp();
            if (c == '\0')
            {
                strcpy(lexeme, "EOF\0");
                logToken(crtToken(T_EOF)); // end of file 
                tokenNo = 0;
                *stop = true;
                break;
            }
            if (c == '\n')
                c = ' ';
            
            Type t = getTChar(c); 
            switch(t)
            {
                case T_NULL:
                    lRec(c);
                    cont = true;
                    break;
                case T_LIT:
                    logToken(crtToken(getTLexeme(lexeme)));
                    readLit();
                    break;
                case T_SEP:
                    logToken(crtToken(getTLexeme(lexeme)));
                    break;
                case T_OP:
                    logToken(crtToken(getTLexeme(lexeme)));
                    break;
                case T_COM:
                    readCom();
                    break;
            }
        }
    }
    return tokens[tokenNo - 1];
}

