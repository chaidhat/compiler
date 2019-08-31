char inFilepath[128];
char outFilepath[128];
int mode;
char dataBuffer[2048];

void preprocess();
void compile();

// io.c
void IoRead (char *filename);
void IoWrite (char *toFilename);
int IoInp (int argc, char* argv[]);

void IoPrint (char* suffix, char* format, va_list args );
void IoLog (char* format, ... );
void IoWarn (char* format, ... );
void IoErr (char* format, ... );
