#define DB_SIZE 2048

char inFilepath[128];
char outFilepath[128];

int mode;

typedef char bool;
// io.c
//   file input handling
typedef struct
{
    int h;
    int line;
} Pos;

Pos pos;
char inp (); // read next char
bool incTC (char expect); // confirm this char

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

typedef unsigned char Type;
typedef struct
{
    Type type;
    char id[128]; // both of these are indexes
    int pos; // for later stages
} Token;

static Token next ();

//   lexemes
static void lRec (char in);
static void lClr ();
static void lInit ();

static Token getToken (char inLexeme[128]);
static void logToken (Token inToken);
static Token crtToken (unsigned char type, char id[128], int pos);
static Token cmpToken (char inLexeme[128], int cmpTable);
static Type cmpChar (char inChar);

static void readCom (int *i);
static void readLit (int *i);

int tokenNo;
Token tokens[DB_SIZE];
