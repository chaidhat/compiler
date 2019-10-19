#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DB_SIZE 2048

enum TokType
{
    T_NULL = 0, // NULL
    T_LIT = 1,  // LITERALS
    T_ID = 2,   // IDENTIFIERS 
    T_KEY = 3,  // KEYWORDS
    T_SEP = 4,  // SEPARATORS
    T_OP = 5,   // OPERATORS
    T_COM = 6,  // COMMENTS
    T_EOF = 7,  // END
}; 
enum LitType
{
    LT_CHAR,
    LT_INT,
};
enum InstType
{
    IT_Var,
    IT_Func,
    IT_Ret,
    IT_Lit,
    IT_Id,
    IT_Assign,
    IT_Deref,
    IT_Ref,
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
};
enum wCodes
{
    WC_PARSE_SEM,
    WC_PARSE_SYN,
};

typedef union
{
    void *tVoid;
    int tInt;
    char tChar;
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
    bool isStatic;
    char varName[128];
} Var;

typedef struct
{
    enum LitType retType;
    bool isPtr;
    bool isStatic;
    char funcName[128];
    int noParameters;
    struct Tree *parameters; // only Var
    int noScope;
    struct Tree *scope;
} Func;
typedef struct
{
    struct Tree *exprsn; // only Exprsn
} Ret;
typedef struct
{
    enum LitType type;
    LitVal val;
} Lit; // is Exprsn
typedef struct
{
    bool isPtr;
    char varName[128];
} Id; // is Exprsn
typedef struct
{
    char varName[128];
    struct Tree *exprsn; // only Exprsn
} Assign;
typedef struct
{
    struct Tree *exprsn; // only Exprsn
} Deref;
typedef struct
{
    struct Tree *ptr; // must be ptr
} Ref;
typedef struct
{
    char funcName[128];
    struct Tree *args; // only Var
} Call;
typedef struct
{
    bool stub;
    bool single;
    Token op;
    struct Tree *left; // only Exprsn
    struct Tree *right; // only Exprsn
} Binary; // is Exprsn
typedef struct
{
    struct Tree *exprsn; // only Exprsn
    struct Tree *scope;
} Cond;
typedef struct
{
    struct Tree *exprsn; // only Exprsn
    struct Tree *scope;
} Ctrl;
typedef struct
{
    char strctName[128];
    struct Tree *decls; // only decl
} Strct;
typedef struct
{
    char uninName[128];
    struct Tree *decls; // only decl
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
        Lit lit;
        Id id;
        Assign assign;
        Deref deref;
        Ref ref;
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
bool isChangeFilepath;


bool mode;
bool doBenchmarking;
bool doRun;
char doRunArgs[128];
bool doParsing;
bool doAssemble;
bool doLinker;
bool doWarnings;
bool doWarningsE;
bool doDumpAst;
bool doDumpSta;

Tree AST;

// file.c
Pos inpPos;
char inp (); // read next char
bool inpCT (char expect); // confirm this char
bool inpCN (char expect); // confirm next char

char inpT; // this char
char inpN; // next char

void inpOpen (char *filename); // opens file
void inpClose ();
void inpWrite (char *toFilename, char *extn); // closes and writes file
void inpPush (char *inDataBuffer);
void inpPop ();
void inpGoto (Pos pos);


// io.c
void mccLog (char* format, ... );
void mccWarn (char* format, ... );
void mccErrC (enum eCodes eCode, char* format, ... );
void mccWarnC (enum wCodes wCode, char* format, ... );
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
void parse ();

// vec.c
void appendChild (Tree *parent, Tree child);
bool deleteChild (Tree *parent, char id[128]);
Tree *getTree (Tree *parent, int index);
void logTree (Tree *t);

int mccStrtod (char *num);
char *mccDtostr (int in);


// dump.c
void dumpPp ();
void dumpAst (Tree *tree);
