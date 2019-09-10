#include <stdarg.h>
#include "btcc.h"

static void btccPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "btcc: %s %s\n", suffix, format);
  vprintf (buf, args);
}

static void btccPrintE (int eCode, char* suffix, char* format, va_list args)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s:E%d: at %d,%d:  %s %s\033[m\n", inFilepath, eCode, inpPos.line, inpPos.h, suffix, format);
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
void btccErrC (int eCode, char* format, ... )
{
    va_list args;
    va_start (args, format);
    char eMsg[128];
    for (int i = 0; i < 128; i++)
        eMsg[i] = '\0';
    strcat(eMsg, "\033[1;31m");
    if (eCode == 0)               { strcat (eMsg, "FATAL UNDEFINED");  }
    if (eCode > 0 && eCode < 100) { strcat (eMsg, "FATAL COMPILER");  }
    if (eCode >= 100 && eCode < 200) { strcat (eMsg, "preprocessor error");  }
    if (eCode >= 200 && eCode < 300) { strcat (eMsg, "lexical error");  }
    strcat(eMsg, ":\033[m\033[1;38m");
    btccPrintE (eCode, eMsg, format, args);
    va_end (args);
    if (eCode < 100) { btccExit(1, 0); }
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
