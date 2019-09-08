#include <stdarg.h>
#include "btcc.h"

void btccPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "btcc: %s %s\n", suffix, format);
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
        btccPrint("\033[1;33mWARNING:\033[m", format, args);
    va_end (args);
}
void btccErr (char* format, ... )
{
    va_list args;
    va_start (args, format);
    btccPrint("\033[1;31mERROR:\033[m", format, args);
    va_end (args);
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
