#define DB_SIZE 2048

char inFilepath[128];
char outFilepath[128];

int mode;
char dataBuffer[DB_SIZE];

// io.c
void IoRead (char *filename);
void IoWrite (char *toFilename);
void IoInp (int argc, char* argv[]);

void IoPrint (char* suffix, char* format, va_list args );
void IoLog (char* format, ... );
void IoWarn (char* format, ... );
void IoErr (char* format, ... );

void IoExit (int code, int debugLine);

// preproc.c
void preprocess ();

// lex.c
void lex ();

typedef struct
{
    unsigned char type;
    char id[128]; // both of these are indexes
    int pos; // for later stages
} Token;

Token getToken (char inLexeme[128]);
void logToken (Token inToken);
Token crtToken (unsigned char type, char id[128], int pos);
Token cmpToken (char inLexeme[128], int cmpTable);
Token cmpChar (char inChar);

int tokenNo;
Token tokens[DB_SIZE];
