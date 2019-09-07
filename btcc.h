#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#define DB_SIZE 2048

char inFilepath[128];
char outFilepath[128];

int mode;

// io.c
//   file input handling
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

void IoOpen (char *filename); // opens file
void IoClose ();
void IoOutput (char *toFilename); // closes and writes file
void IoUI (int argc, char* argv[]);

void IoPrint (char* suffix, char* format, va_list args );
void IoLog (char* format, ... );
void IoWarn (char* format, ... );
void IoErr (char* format, ... );

void IoExit (int code, int debugLine);

// preproc.c
void preprocess ();

// lex.c
void LexInit ();
void lex ();

typedef struct
{
    unsigned char type;
    /* TOKEN TYPES
    *  0 - NULL
    *  1 - LITERALS
    *  2 - IDENTIFIERS
    *  3 - KEYWORDS
    *  4 - SEPARATORS
    *  5 - OPERATORS
    */
    char id[128]; // both of these are indexes
    Pos pos; // for later stages
} Token;

Token next ();

int tokenNo;
Token tokens[DB_SIZE];
