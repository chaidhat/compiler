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
    snprintf(buf, sizeof buf, "%s: %s on %d;%d,%d: %s\033[m\n", inFilepath, suffix, inpPos.total, inpPos.line, inpPos.h,  format);
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

void mccDoArgs (int argc, char* argv[])
{
    if (argc > 1)
    {
        for (int line = 1; line < argc; line++)
        {
            char* args[] =
            {
                "-V",
                "-b",
                "-o",
                "-r",
                "-v",
                "-w",
                "-I",
                "-D",
                "-U",
                "-E",
                "-S",
                "-c",
                "-h",
            };
            int argNo = -1;
            for (int i = 0; i < sizeof(args)/sizeof(args[0]); i++)
            {
                if (strncmp(argv[line],args[i],strlen(args[i])) == 0)
                {
                    argNo = i;
                    break;
                }
            }
             
            switch (argNo)
            {
                case 0:
                    printf("\033[1;30mMinimal-C Compiler created by Chaidhat Chaimongkol\033[m\n"
                    "compiled on: %s %s\n"
                    "\n"
                    , __DATE__, __TIME__);
                    mccExit(2, __LINE__);
                    break;
                case 1:
                    doBenchmarking = true; 
                    break; 
                case 2:
                    strcpy(outFilepath, argv[line + 1]);
                    line++;
                    break;
                case 3:
                    doRun = true;
                    strcpy(doRunArgs, argv[line + 1]);
                    line++;
                    break;
                case 4:
                    mode = 0;
                    break;
                case 5:
                    doWarnings = false;
                    break;
                case 6:
                    predefineInclude(argv[line + 1]);
                    line++;
                    break;
                case 7:
                    predefineMacro(argv[line + 1], argv[line + 2]);
                    line += 2;
                    break;
                case 8:
                    predefineMacro(argv[line + 1], 0);
                    line++;
                    break;
                case 9:
                    doParsing = false;
                    break;
                case 10:
                    doAssemble = false;
                    break;
                case 11:
                    doLinker = false;
                    break;
                case 12:
                    printf("\033[1;30mMinimal-C Compiler created by Chaidhat Chaimongkol\n%s %s\n\n\033[m", __DATE__, __TIME__);
                    printf("usage: mcc [-h] [-V] [arg1 arg2 ...] <inpath1 inpath2 ...>\n\n" 
                    "args:\n"
                    "   -V                  display version info\n"
                    "   -b                  display compilation stats once end\n"
                    "   -o <path>           write output to <path>\n"
                    "   -r <args ...>       run output\n"
                    "   -v                  verbose compiler debugger\n"
                    "   -w                  supress all warnings\n"
                    "\n"
                    "   -I <dir>            add include path <dir>\n"
                    "   -D <macro> <val>    set <macro> to <val>\n"
                    "   -U <macro>          undefine <macro>\n"
                    "   -E                  stop after preproccessing\n"
                    "\n"
                    "   -S                  stop after parsing. do not asm\n"
                    "   -c                  do not link\n"
                    "\n"
                    "   -h                  display this help\n"
                    "\n"
                    "inpath only accepts .mc\n"
                    );
                    mccExit(2, __LINE__);
                    break;
                default:
                    strcpy(inFilepath, argv[line]);
                    char mcName[100];
                    int i = 0;
                    int j = 0;
                    while (argv[line][i] != '.')
                        i++;
                    if (argv[line][i + 1] != 'm' || argv[line][i + 2] != 'c')
                    {
                        mccErr("Unexpected file type (expected .mc)\n ./mcc -h for help");
                        mccExit(1, __LINE__);
                    }
                    while (i > 0)
                    {
                        i--;
                        j++;
                    }
                    mcName[j] = '\0';
                    while (j > 0)
                    {
                        j--;
                        mcName[j] = argv[line][i + j];
                    }

                    if (strncmp(outFilepath,"$",strlen("$")) == 0)
                        strcpy(outFilepath, mcName);
                    strcat(outFilepath, ".o");
                    break;
            }
        }
    }
    else
    {
        mccErr("no input arguments.\n ./mcc -h for help");
        mccExit(2, __LINE__);
    }
}
