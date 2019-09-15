#include <stdarg.h>
#include "mcc.h"

static void mccPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "mcc: %s %s\n", suffix, format);
  vprintf (buf, args);
}

static void mccPrintE (char* suffix, char* format, va_list args)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s: %s on %d,%d: %s\033[m\n", inFilepath, suffix, inpPos.line, inpPos.h,  format);
  vprintf (buf, args);
}

void mccLog (char* format, ... )
{
    va_list args;
    va_start (args, format);
    if (mode == 0)
        mccPrint("", format, args);
    va_end (args);
}
void mccWarn (char* format, ... )
{
    va_list args;
    va_start (args, format);
    if (doWarnings)
        mccPrint("\033[1;33mwarning:\033[m", format, args);
    va_end (args);
}
void mccErr (char* format, ... )
{
    va_list args;
    va_start (args, format);
    mccPrint("\033[1;31merror:\033[m", format, args);
    va_end (args);
}
void mccErrC (enum eCodes eCode, char* format, ... )
{
    va_list args;
    va_start (args, format);
    char eMsg[128];
    for (int i = 0; i < 128; i++)
        eMsg[i] = '\0';
    strcat(eMsg, "\033[1;31m");
    if (eCode == EC_FATAL)               { strcat (eMsg, "FATAL ERROR");  }
    if (eCode == EC_LEX) { strcat (eMsg, "lexical error");  }
    if (eCode == EC_PP) { strcat (eMsg, "preprocessor error");  }
    if (eCode == EC_PARSE) { strcat (eMsg, "parsing error");  }
    strcat(eMsg, ":\033[m\033[1;38m");
    mccPrintE (eMsg, format, args);
    va_end (args);
    if (eCode == EC_FATAL) { mccExit(1, 0); }
}

void mccExit (int code, int debugLine)
{
    switch (code)
    {
        case 0:
            mccLog("Terminated sucessfully.");
            break;
        case 1:
            mccLog("Terminated UNsucessfully.");
            if (mode == 0)
                mccLog("Called from line %d", debugLine);
            break;
        case 2:
            if (mode == 0)
                mccLog("Terminated calm. Called from line %d", debugLine);
            break;
    }
    exit(0);
}
