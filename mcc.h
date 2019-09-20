#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DB_SIZE 2048

// thank you to
// compiler - http://www.cs.man.ac.uk/~pjj/farrell/compmain.html 
// lexing -   http://www.cse.chalmers.se/edu/year/2015/course/DAT150/lectures/proglang-04.html

char inFilepath[128];
char outFilepath[128];

// in args
char predefPP[DB_SIZE];

bool mode;
bool doBenchmarking;
bool doRun;
char doRunArgs[128];
bool doParsing;
bool doAssemble;
bool doLinker;
bool doWarnings;

// file.c
typedef struct
{
    int h;
    int line;
} Pos;

Pos inpPos;
char inp (); // read next char
bool inpCT (char expect); // confirm this char
bool inpCN (char expect); // confirm next char

char inpT; // this char
char inpN; // next char

void inpOpen (char *filename); // opens file
void inpClose ();
void inpOutput (char *toFilename); // closes and writes file

// io.c
enum eCodes
{
    EC_FATAL,
    EC_LEX,
    EC_PP,
    EC_PARSE,
};
void mccLog (char* format, ... );
void mccWarn (char* format, ... );
void mccErrC (enum eCodes eCode, char* format, ... );
void mccErr (char* format, ... );
void mccDoArgs (int argc, char *argv[]);

void mccExit (int code, int debugLine);

// lex.c
enum TokType
{
    T_NULL, // NULL
    T_LIT,  // LITERALS
    T_ID,   // IDENTIFIERS
    T_KEY,  // KEYWORDS
    T_SEP,  // SEPARATORS
    T_OP,   // OPERATORS
    T_COM,  // COMMENTS
    T_EOF,  // END
};

typedef struct
{
    enum TokType type;
    char id[128]; // both of these are indexes
    Pos pos; // for later stages
} Token;

Token lex ();

bool isEOF;
Token tokT;

bool tokcmpType (enum TokType type);
bool tokcmpId (char *id);

Token tokens[DB_SIZE];

// pp.c
void readDefine ();
void readIf ();

// parse.c
void next ();

// vec.c
typedef struct Tree
{
    char id[128];
    struct Tree **children; // neat self-referential struct
    int noChild;
} Tree;
Tree *crtTree (char *id);
void appendTree (Tree *parent, Tree *child);
bool deleteTree (Tree *parent, char id[128]);
Tree *getTree (Tree *parent, int index);
void logTree (Tree *t);
