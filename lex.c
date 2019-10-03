#include <ctype.h>
#include "mcc.h"


static char lexeme[128];
Token tokens[DB_SIZE];

static const Token TOKEN_NULL = {T_NULL,"NULL",0};

static int i = 0, j = 0;
static int tokenNo = 0, tokenNoActual = 0;
static bool isEOF = false;
static bool isAstr = false;
static Token tokT;

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
    "char",
    "int",
    "struct",
    "union",
    "void",
    "return",
    "if",
    "while",
};

static const char ReSep[] =
{
    ' ',
    '#',
    '.',
    ',',
    ';',
    '(',
    ')',
    '[',
    ']',
    '{',
    '}',
    '"',
};

static const char ReOp[] =
{
    '=',
    '+',
    '-',
    '*',
    '/',
    '&',
    '>',
    '!',
    '|',
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
    if (inChar == '/' && (inpCN('/') || inpCN('*')))
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
    if ((strlen(inToken.id) < 1 || strcmp(inToken.id, " ") == 0) && !(inToken.type == T_EOF))
    {
        return false;
    }
    tokens[tokenNo] = inToken; 
    tokenNoActual++;
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
    tokenNoActual++;
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
    if (inpN == '*')
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
    inpT = ' ';
}

bool tokcmpType (enum TokType type)
{
    if (tokT.type == type)
        return true;
    return false;
}

bool tokcmpId (char *id)
{
    if (strcmp(tokT.id, id) == 0)
        return true;
    return false;
}

bool tTokcmpType (Token tok, enum TokType type)
{
    if (tok.type == type)
        return true;
    return false;
}

bool tTokcmpId (Token tok, char *id)
{
    if (strcmp(tok.id, id) == 0)
        return true;
    return false;
}

void resetEOF()
{
    tokens[tokenNo - 1].type = T_NULL;
    tokT = tokens[tokenNo - 1];
    isEOF = false;
}
Token lex ()
{
    if (tokenNo < tokenNoActual)
    {
        tokenNo++;
        tokT = tokens[tokenNo - 1];
        return tokT;
    }
    bool cont = true;
    while (cont)
    {
        // log char
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
            tokenNoActual = 0;
            isEOF = true;
            logToken(crtToken(T_EOF));
            break;
        }
        if (c == '\n')
            c = ' ';

        enum TokType t = getTChar(c); 

        char l[128];
        strcpy(l, lexeme);
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
                strcpy(lexeme, ppLexeme(l));
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

Token peek ()
{
    return tokT;
}

Token unlex ()
{
    tokenNo--;
    tokT = tokens[tokenNo - 1];
    return tokT;
}

char *getLit ()
{
    if (!tokcmpType(T_LIT))
        mccErrC(EC_PARSE_SYN, "expected literal");
    return tokT.id;
}

char *getId ()
{
    if (!tokcmpType(T_ID))
        mccErrC(EC_PARSE_SYN, "expected identifier");
    return tokT.id;
}

bool isLit ()
{
    return tokcmpType(T_LIT);
}

bool isId ()
{
    return tokcmpType(T_ID);
}

bool isKw (char *id)
{
    return tokcmpType(T_KEY) && tokcmpId(id);
}

bool isSep (char *id)
{
    return tokcmpType(T_SEP) && tokcmpId(id);
}

bool isOp (char *id)
{
    return tokcmpType(T_OP) && tokcmpId(id);
}
