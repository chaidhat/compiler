#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DB_SIZE 2048

// thank you to
// compiler - http://www.cs.man.ac.uk/~pjj/farrell/compmain.html 
//          - http://lisperator.net/pltut/
// parser   - http://lisperator.net/pltut/parser/
//          - https://stackoverflow.com/questions/2245962/is-there-an-alternative-for-flex-bison-that-is-usable-on-8-bit-embedded-systems/2336769#2336769
// lexing -   http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html

enum TokType
{
    T_NULL = 0, // NULL
    T_LIT = 1,  // LITERALS T_ID = 2,   // IDENTIFIERS T_KEY = 3,  // KEYWORDS
    T_ID = 2,
    T_KEY = 3,
    T_SEP = 4,  // SEPARATORS
    T_OP = 5,   // OPERATORS
    T_COM = 6,  // COMMENTS
    T_EOF = 7,  // END
}; 
enum LitType
{
    LT_Void,
    LT_Int,
    LT_Char,
};
enum InstType
{
    IT_Var,
    IT_Func,
    IT_Ret,
    IT_Literal,
    IT_Id,
    IT_Assign,
    IT_Ptr,
    IT_Addr,
    IT_Call,
    IT_Binary,
    IT_Cond,
    IT_While,
    IT_Struct,
    IT_Union,
    IT_Scope,
};
enum eCodes
{
    EC_FATAL,
    EC_LEX,
    EC_PP,
    EC_PARSE_SEM,
    EC_PARSE_SYN,
};

typedef union
{
    void *LT_Void;
    int LT_Int;
    char LT_Char;
} LitVal;

typedef struct
{
    int h;
    int line;
    int total;
} Pos;

typedef struct
{
    enum TokType type;
    char id[128]; // both of these are indexes
    Pos pos; // for later stages
} Token;

typedef struct Tree
{
    char id[128];
    enum InstType type;
    union
    {
        void *null;
        struct
        {
            enum LitType varType; 
            char varName[128];
        } Var;
        struct
        {
            enum LitType retType;
            char funcName[128];
            struct Tree *parameters;
            struct Tree *scope;
        } Func;
        struct
        {
            struct Tree *retval;
        } Ret;
        struct
        {
            enum LitType type;
            LitVal val;
        } Literal;
        struct
        {
            enum LitType type;
            char varName[128];
        } Id;
        struct
        {
            char varName[128];
            struct Tree *val;
        } Assign;
        struct
        {
            struct Tree *var;
        } Ptr;
        struct
        {
            struct Tree *ptr;
        } Addr;
        struct
        {
            char funcName[128];
            struct Tree *args;
        } Call;
        struct
        {
            struct Tree *left;
            struct Tree *right;
            char operation;
        } Binary;
        struct
        {
            struct Tree *conditional;
            struct Tree *scope;
        } Cond;
        struct
        {
            struct Tree *conditional;
            struct Tree *scope;
        } While;
        struct
        {
            char structName[128];
            struct Tree *vars;
        } Struct;
        struct
        {
            char unionName[128];
            struct Tree *vars;
        } Union;
        struct
        {
            struct Tree *block;
        } Scope;
    } Inst;
    struct Tree *children; // neat self-referential struct
    int noChild;
} Tree;


char startFilepath[128];
char inFilepath[128];
char outFilepath[128];


bool mode;
bool doBenchmarking;
bool doRun;
char doRunArgs[128];
bool doParsing;
bool doAssemble;
bool doLinker;
bool doWarnings;


// file.c
Pos inpPos;
char inp (); // read next char
bool inpCT (char expect); // confirm this char
bool inpCN (char expect); // confirm next char

char inpT; // this char
char inpN; // next char

void inpOpen (char *filename); // opens file
void inpClose ();
void inpWrite (char *toFilename); // closes and writes file
void inpPush (char *inDataBuffer);
void inpGoto (Pos pos);


// io.c
void mccLog (char* format, ... );
void mccWarn (char* format, ... );
void mccErrC (enum eCodes eCode, char* format, ... );
void mccErr (char* format, ... );
void mccDoArgs (int argc, char *argv[]);

void mccExit (int code, int debugLine);


// lex.c
void resetEOF();

Token lex ();
Token peek ();
Token unlex ();

bool tokcmpType (enum TokType type);
bool tokcmpId (char *id);
bool tTokcmpType (Token tok, enum TokType type);
bool tTokcmpId (Token tok, char *id);

bool isLit ();
bool isId ();
bool isKw (char *id);
bool isSep (char *id);
bool isOp (char *id);


// pp.c
void ppInit ();
char *ppLexeme (char *lexeme);
Token ppToken (Token token);

void predefineMacro (char *name, char *val);
void predefineInclude (char *dir);


// parse.c
void next ();
void parse (Token t);


// vec.c
Tree crtTree (char *id);
void appendChild (Tree *parent, Tree child);
bool deleteChild (Tree *parent, char id[128]);
Tree *getTree (Tree *parent, int index);
void logTree (Tree *t);
