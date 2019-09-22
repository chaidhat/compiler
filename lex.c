#include <ctype.h>
#include "mcc.h"

const Token TOKEN_NULL = {0,"NULL",0};

static char lexeme[128];
Token tokens[DB_SIZE];

static int i = 0, j = 0;
static int tokenNo = 0;

//   lexemes
static void lRec (char in);
static void lClr ();
static void lInit ();

static enum TokType getTLexeme (char inLexeme[128]);
static enum TokType getTChar (char inChar);

static bool logToken (Token inToken);
static bool logChar (char inChar);

static Token crtToken (enum TokType type);

static void readLit ();
static void readCom ();

// regular expressions
static const char* ReKeywords[] =
{
    "define",
    "include",
    "ifdef",
    "endif",
    "a",
};

static const char ReSep[] =
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

static const char ReOp[] =
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



static enum TokType getTLexeme (char inLexeme[128])
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

static enum TokType getTChar (char inChar)
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

static bool logToken (Token inToken)
{
    lClr();
    // check for no tokens or spaces, do not record
    if (strlen(inToken.id) < 1 || strcmp(inToken.id, " ") == 0)
    {
        return false;
    }
    tokens[tokenNo] = inToken; 
    tokenNo++;
    return true;
}

static bool logChar (char inChar)
{
    lClr();
    if (inChar == ' ' || inChar == '"')
        return false;
    Token inToken = crtToken(getTChar(inChar));
    strcpy(inToken.id, " ");
    inToken.id[0] = inChar;
    tokens[tokenNo] = inToken; 
    tokenNo++;
    return true;
}


static Token crtToken (enum TokType type)
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
        mccErrC(EC_LEX, "unterminated '\"'");
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
    else if (inpN == '*')
    {
        while (!(inpCT('*') && inpCN('/')) && !inpCT('\0'))
        {
            inp();
            if (inpCT('/') && inpCN('*'))
            {     
                inpPos = p;
                mccErrC(EC_LEX, "overlapping '/*'. Did not expect '/*'");
            }
        }
        if (inpCT('\0'))
        {
            inpPos = p;
            mccErrC(EC_LEX, "unterminated '/*'. Expecting '*/' to close '/*'");
        }
        inp();
    }
    else
    {
        mccErrC(EC_LEX, "unexpected '%c'. Expecting '/' or '*' for comment", inpN);
    }
    inpT = ' ';
}

bool tokcmpType (enum TokType type)
{
    Token t = tokens[tokenNo - 1];
    if (tokens[tokenNo - 1].type == type)
        return true;
    return false;
}

bool tokcmpId (char *id)
{
    if (strcmp(tokens[tokenNo - 1].id, id) == 0)
        return true;
    return false;
}

Token lex ()
{
    bool cont = true;
    while (cont)
    {
        if (getTChar(inpT) != T_NULL && inpT != '\0')
        {
            if(logChar(inpT))
            {
                inpT = '\0'; 
                break;
            }
        }
        cont = false;
        char c = inp();

        // preprocessing
        if (c == '\0')
        {
            lInit();
            j = -1;
            tokenNo = 0;
            isEOF = true;
            return crtToken(T_EOF);
        }
        if (c == '\n')
            c = ' ';
        
        enum TokType t = getTChar(c); 
        switch(t)
        {
            case T_NULL:
                lRec(c);
                cont = true;
                break;
            case T_LIT:
                readLit();
                break;
            case T_SEP:
                if (!logToken(crtToken(getTLexeme(lexeme))))
                    cont = true;
                break;
            case T_OP:
                if (!logToken(crtToken(getTLexeme(lexeme))))
                    cont = true;
                break;
            case T_COM:
                readCom();
                cont = true;
                break;
            default: // to silence warnings
                break;
        }
    }
    tokT = tokens[tokenNo - 1];
    return tokT; 
}

