#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DB_SIZE 65536
#define mccLog( ... ) debugLog(__FILE__, __LINE__, __VA_ARGS__) 
#define mccWarn( ... ) debugWarn(__FILE__, __LINE__, __VA_ARGS__) 
#define mccErr( ... ) debugErr(__FILE__, __LINE__, __VA_ARGS__) 
#define mccExit(code) debugExit(__FILE__, __LINE__, code) 
#define mccBr() debugLog(__FILE__, __LINE__, "breakpoint\n")
#define mccstrs(dest, format, ... ) mccstr(dest, 128, format, __VA_ARGS__)

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
    LT_INVALID = 0,
    LT_CHAR = 1,
    LT_INT = 2,
    // user defined types are 2 and above
};
enum InstType
{
    IT_Var,
    IT_Func,
    IT_Lit,
    IT_Id,
    IT_Ret,
    IT_Call,
    IT_Assign,
    IT_Deref,
    IT_Ref,
    IT_Binary,
    IT_Cond,
    IT_Ctrl,
    IT_Strct,
    IT_Unin,
    IT_Scope,
};
enum DagType
{
    DT_func,
    DT_operand,
    DT_str,
    DT_num,
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
enum OpcodeInstType
{
    OP_push,
    OP_pop,
    OP_mov,
    OP_cmp,
    OP_jmp,
    OP_call,
};
enum OpcodeMemType
{
    OST_word,
    OST_long,
};
enum RegType
{
    REG_a,
    REG_b,
    REG_c,
    REG_d,
    REG_esi,
    REG_edi,
    REG_ebp,
    REG_esp,
    REG_USE_REGNAME
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
    char varName[128];
    bool isPtr;
    bool isStatic;
    bool isArray;
    struct Tree *arrayLength; // only Exprsn
} Var;

typedef struct
{
    enum LitType retType;
    bool isPtr;
    bool isStatic;
    char funcName[128];
    struct Tree *parameters; // only Var
    struct Tree *scope;
} Func;
typedef struct
{
    enum LitType type;
    LitVal val;
} Lit; // is Exprsn
typedef struct
{
    bool isPtr;
    char varName[128];
    struct Tree *nested; // only Id
} Id; // is Exprsn
typedef struct
{
    struct Tree *exprsn; // only Exprsn
} Ret;
typedef struct
{
    char funcName[128];
    struct Tree *args; // only Var
} Call;
typedef struct
{
    struct Tree *varName; // only Id or Deref
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
    char scopeName[128];
    struct Tree *decls; // only decl
} Scope;

typedef struct
{
    // LITERALS
    bool isLit; // is $ ?
    bool isRoutine; // is it lit without $?
    char *name; // use ONLY if lit or routine 

    // REGISTERS / ADDR
    enum RegType type;
    int regnum; // used for infinite registers
    int offset; // offset from register e.g. 4 in 4(%eax)
} Operand;

typedef struct
{
    enum OpcodeInstType type; // e.g. push, mov
    enum OpcodeMemType size; // byte, long
} Opcode;

typedef struct
{
    enum InstType type;
    union
    {
        Var var;
        Func func;
        Lit lit;
        Id id;
        Assign assign;
        Ret ret;
        Call call;
        Deref deref;
        Ref ref;
        Binary binary;
        Cond cond;
        Ctrl ctrl;
        Strct strct;
        Unin unin;
        Scope scope;
        void *null;
    }; // anonymous union
} AstInst;

typedef struct
{
    enum DagType type;
    union
    {
        Operand operand;
        char str[128];
        int num;
    };
} DagInst; // DAG IR

typedef struct
{
    Opcode op;

    Operand dest; // left-hand side of operand
    Operand src; // right-hand side optional, depending on RepType

    //int lifetime; // for optimisation
} IrInst; // linear IR

typedef struct Tree
{
    char id[128];
    
    union
    {
        AstInst ast; // for AST
        DagInst dag; // for IR as DAG
    };

    struct Tree *children; // neat self-referential struct
    int childrenSz; // if IR as DAG, it is assumed at 2 as a bin tree
} Tree;

typedef struct
{
    char name[128];

    IrInst *irInst;
    int irInstSz;
} IrRoutine; // for linear IR


// file vars
char startFilepath[128];
char inFilepath[128];
char outFilepath[128];
bool isChangeFilepath;


// config vars
bool mode;
bool doBenchmarking;
bool doParsing;
bool doAssemble;
bool doLink;
bool doWarnings;
bool doWarningsE;
bool doDumpAst;


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

void inpGetFilename (char *outFilename, int outFilenameSz);


// debug.c
void debugLog (char *filename, int line, char* format, ... );
void debugWarn (char *filename, int line, char* format, ... );
void debugErr (char *filename, int line, char* format, ... );
void mccErrC (enum eCodes eCode, char* format, ... );
void mccWarnC (enum wCodes wCode, char* format, ... );
void mccDoArgs (int argc, char *argv[]);

void debugExit (char *filename, int line, int code);


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
void parse (Tree *AST);


// vec.c
void appendChild (Tree *parent, Tree child);
bool deleteChild (Tree *parent, char id[128]);
Tree *getTree (Tree *parent, int index);
void logTree (Tree *t);

int mccstrtod (char *num);
char *mccdtostr (int in);
bool mccstr (char *dest, int destSz, char *format, ... );


// dump.c
void dumpPp ();
void dumpAst (Tree *AST);


// gen_ir.c
void genIr (Tree *IrDag, Tree *AST);


// map.c
void map (IrRoutine *IrLinear, Tree *IrDag);


// gen_x86.c
void genX (char *dest, int destSz, IrRoutine *IrLinear);
