#define DB_SIZE 2048

char inFilepath[128];
char outFilepath[128];

int tokenNo;
int mode;
char tokens[DB_SIZE][128];
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

int checkOperator (char *inToken);
void logToken (char inToken[128]);
