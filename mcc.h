#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define DB_SIZE 2048

// thank you to
// compiler - http://www.cs.man.ac.uk/~pjj/farrell/compmain.html 
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
enum eCodes
{
    EC_FATAL,
    EC_LEX,
    EC_PP,
    EC_PARSE,
};

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
    struct Tree *children; // neat self-referential struct
    int noChild;
} Tree;


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
void inpAppend (char *inDataBuffer);
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


// pp.c

void ppInit ();
char *ppLexeme (char *lexeme);

void predefineMacro (char *name, char *val);
void predefineInclude (char *dir);

void next ();

// parse.c
void parse (Token t);

// vec.c
Tree crtTree (char *id);
void appendChild (Tree *parent, Tree child);
bool deleteChild (Tree *parent, char id[128]);
Tree *getTree (Tree *parent, int index);
void logTree (Tree *t);
