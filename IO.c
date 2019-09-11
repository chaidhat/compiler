#include <stdarg.h>
#include "btcc.h"

static void btccPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "btcc: %s %s\n", suffix, format);
  vprintf (buf, args);
}

static void btccPrintE (char* suffix, char* format, va_list args)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s: %s on %d,%d: %s\033[m\n", inFilepath, suffix, inpPos.line, inpPos.h,  format);
  vprintf (buf, args);
}

void btccLog (char* format, ... )
{
    va_list args;
    va_start (args, format);
    if (mode == 0)
        btccPrint("", format, args);
    va_end (args);
}
void btccWarn (char* format, ... )
{
    va_list args;
    va_start (args, format);
    if (doWarnings)
        btccPrint("\033[1;33mwarning:\033[m", format, args);
    va_end (args);
}
void btccErr (char* format, ... )
{
    va_list args;
    va_start (args, format);
    btccPrint("\033[1;31merror:\033[m", format, args);
    va_end (args);
}
void btccErrC (enum eCodes eCode, char* format, ... )
{
    va_list args;
    va_start (args, format);
    char eMsg[128];
    for (int i = 0; i < 128; i++)
        eMsg[i] = '\0';
    strcat(eMsg, "\033[1;31m");
    if (eCode == FATAL)               { strcat (eMsg, "FATAL ERROR");  }
    if (eCode == LEX) { strcat (eMsg, "lexical error");  }
    if (eCode == PP) { strcat (eMsg, "preprocessor error");  }
    if (eCode == PARSE) { strcat (eMsg, "parsing error");  }
    strcat(eMsg, ":\033[m\033[1;38m");
    btccPrintE (eMsg, format, args);
    va_end (args);
    if (eCode == FATAL) { btccExit(1, 0); }
}

void btccExit (int code, int debugLine)
{
    switch (code)
    {
        case 0:
            btccLog("Terminated sucessfully.");
            break;
        case 1:
            btccLog("Terminated UNsucessfully.");
            if (mode == 0)
                btccLog("Called from line %d", debugLine);
            break;
        case 2:
            if (mode == 0)
                btccLog("Terminated calm. Called from line %d", debugLine);
            break;
    }
    exit(0);
}
