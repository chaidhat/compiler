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
#define T_NULL 0
#define T_LIT 1
#define T_ID 2
#define T_KEY 3
#define T_SEP 4
#define T_OP 5
#define T_COM 6
#define T_EOF 7

typedef unsigned char Type;

typedef struct
{
    Type type;
    /* TOKEN TYPES
    *  0 - NULL
    *  1 - LITERALS
    *  2 - IDENTIFIERS
    *  3 - KEYWORDS
    *  4 - SEPARATORS
    *  5 - OPERATORS
    *  6 - COMMENTS
    *  7 - EOF
    */
    char id[128]; // both of these are indexes
    Pos pos; // for later stages
} Token;

Token lex ();

bool isEOF;

Token tokT;

bool tokcmpType (Type type);
bool tokcmpId (char *id);

Token tokens[DB_SIZE];

// pp.c
void readDefine ();
void readIf ();

// parse.c
void next ();
