#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DB_SIZE 2048

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
    LT_CHAR,
    LT_INT,
    LT_VOID,
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
    IT_Ctrl,
    IT_Strct,
    IT_Unin,
};
enum eCodes
{
    EC_FATAL,
    EC_LEX,
    EC_PP,
    EC_PARSE_SEM,
    EC_PARSE_SYN,
    EC_PARSE_SYN_FAT,
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

typedef struct
{
    enum LitType varType; 
    bool isPtr;
    char varName[128];
} Var;

typedef struct
{
    enum LitType retType;
    bool isPtr;
    char funcName[128];
    struct Tree *parameters; // only Var
    struct Tree *scope;
} Func;
typedef struct
{
    struct Tree *retval; // only Exprsn
} Ret;
typedef struct
{
    enum LitType type;
    bool isPtr;
    LitVal val;
} Literal; // is Exprsn
typedef struct
{
    enum LitType type;
    bool isPtr;
    char varName[128];
} Id; // is Exprsn
typedef struct
{
    char varName[128];
    struct Tree *val; // only Exprsn
} Assign;
typedef struct
{
    Id id;
} Ptr;
typedef struct
{
    Ptr ptr;
} Addr;
typedef struct
{
    char funcName[128];
    struct Tree *args; // only Var
} Call;
typedef struct
{
    struct Tree *left; // only Exprsn
    struct Tree *right; // only Exprsn
    char operation;
} Binary; // is Exprsn
typedef struct
{
    struct Tree *conditional; // only Exprsn
    struct Tree *scope;
} Cond;
typedef struct
{
    struct Tree *conditional; // only Exprsn
    struct Tree *scope;
} Ctrl;
typedef struct
{
    char structName[128];
    struct Tree *vars; // only decl
} Strct;
typedef struct
{
    char unionName[128];
    struct Tree *vars; // only decl
} Unin;
typedef struct
{
    struct Tree *block;
} Scope;

typedef struct Tree
{
    char id[128];
    enum InstType type;
    union
    {
        Var var;
        Func func;
        Ret ret;
        Literal literal;
        Id id;
        Assign assign;
        Ptr ptr;
        Addr addr;
        Call call;
        Binary binary;
        Cond cond;
        Ctrl ctrl;
        Strct strct;
        Unin unin;
        Scope scope;
        void *null;
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

char *getLit ();
char *getId ();
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
Token next ();
Token prev ();
void parse ();

// vec.c
Tree crtTree (char *id);
void appendChild (Tree *parent, Tree child);
bool deleteChild (Tree *parent, char id[128]);
Tree *getTree (Tree *parent, int index);
void logTree (Tree *t);
