#include <stdarg.h>
#include "mcc.h"

// http://www.codebind.com/cprogramming/get-current-directory-using-c-program/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#define OS "WINDOWS" 
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define OS "MACOSX/LINUX" 
#endif

static void mccPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "mcc: %s %s\n", suffix, format);
  vprintf (buf, args);
}

static void mccPrintE (char* suffix, char* format, va_list args)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s: %s at (%d,%d):\n    %s\033[m\n", inFilepath, suffix, inpPos.line, inpPos.h,  format);
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
    if (eCode == EC_FATAL)               { strcat (eMsg, "*** INTERNAL FATAL ERROR ***"); }
    if (eCode == EC_LEX) { strcat (eMsg, "lex error");  }
    if (eCode == EC_PP) { strcat (eMsg, "preprocess error");  }
    if (eCode == EC_PARSE_SEM) { strcat (eMsg, "semantic error");  }
    if (eCode == EC_PARSE_SYN) { strcat (eMsg, "syntax error");  }
    strcat(eMsg, ":\033[m\033[1;38m");
    mccPrintE (eMsg, format, args);
    va_end (args);
    mccExit(1,-1);
}

void mccWarnC (enum wCodes wCode, char* format, ... )
{
    va_list args;
    va_start (args, format);
    char wMsg[128];
    for (int i = 0; i < 128; i++)
        wMsg[i] = '\0';
    strcat(wMsg, "\033[1;33m");
    if (wCode == WC_PARSE_SEM) { strcat (wMsg, "syntax warning"); }
    if (wCode == WC_PARSE_SYN) { strcat (wMsg, "semantic warning");  }
    strcat(wMsg, ":\033[m\033[1;38m");
    if (doWarnings)
    mccPrintE (wMsg, format, args);
    if (doWarningsE)
        mccExit(1, -2);
    va_end (args);
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
                "-We",
                "-I",
                "-D",
                "-E",
                "-S",
                "-c",
                "-fdast",
                "-fdsta",
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
             
            char homedir[FILENAME_MAX];
            GetCurrentDir( homedir, FILENAME_MAX );
            switch (argNo)
            {
                case 0: // -V
                    printf("\033[1;30mMinimalistiC Compiler created by Chaidhat Chaimongkol\033[m\n"
                    "https://github.com/Chai112/MinC-Compiler\n"
                    "Compiled on: %s %s\n"
                    "Target: x86_64 Intel %s\n"
                    "Installed with: gcc %s\n"
                    "Installed dir: %s\n"
                    "\n"
                    , __DATE__, __TIME__, OS, __VERSION__, homedir);
                    mccExit(2, __LINE__);
                    break;
                case 1: // -b
                    doBenchmarking = true; 
                    break; 
                case 2: // -o
                    strcpy(outFilepath, argv[line + 1]);
                    isChangeFilepath = true;
                    line++;
                    break;
                case 3: // -r
                    doRun = true;
                    strcpy(doRunArgs, argv[line + 1]);
                    line++;
                    break;
                case 4: // -v
                    mode = 0;
                    break;
                case 5: // -w
                    doWarnings = false;
                    break;
                case 6: // -We
                    doWarningsE = true;
                    break;
                case 7: // -I
                    predefineInclude(argv[line + 1]);
                    line++;
                    break;
                case 8: // -D
                    predefineMacro(argv[line + 1], argv[line + 2]);
                    line += 2;
                    break;
                case 9: // -E
                    doParsing = false;
                    break;
                case 10: // -S
                    doAssemble = false;
                    break;
                case 11: // -c
                    doLinker = false;
                    break;
                case 12: // -fdast
                    doDumpAst = true;
                    break;
                case 13: // -fdsta
                    doDumpSta = true;
                    break;
                case 14: // -h
                    printf("\033[1;30mMinimalistiC Compiler created by Chaidhat Chaimongkol\n%s %s\n\033[m", __DATE__, __TIME__);
                    printf("https://github.com/Chai112/MinC-Compiler\n\n"
                    "usage: mcc [-h] [-V] [arg1 arg2 ...] <inpath1 inpath2 ...>\n\n" 
                    "mcc only accepts .minc as inpath, dir and include accepts any types\n"
                    "args:\n"
                    "   -V                  display version info\n"
                    "   -b                  display compilation benchmarks\n"
                    "   -o <path>           write output to <path>\n"
                    "   -r <args ...>       run output with <args ...>\n"
                    "\n"
                    "   -v                  verbose compiler debugger\n"
                    "   -w                  supress all warnings\n"
                    "   -We                 treat warnings as errors\n"
                    "\n"
                    "   -I <dir>            add include path <dir>\n"
                    "   -D <macro> <val>    set <macro> to <val>\n"
                    "   -E                  preprocess only\n"
                    "\n"
                    "   -S                  stop before assembly\n"
                    "   -c                  stop before linking\n"
                    "\n"
                    "   -fdast              dump AST\n"
                    "   -fdsta              dump stack-trace\n"
                    "\n"
                    "   -h                  display this help\n"
                    "\n"
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
                    if (argv[line][i + 1] != 'm' || argv[line][i + 2] != 'i' ||  argv[line][i + 3] != 'n' || argv[line][i + 4] != 'c') 
                    {
                        mccErr("Unexpected file type (expected .minc)\n ./mcc -h for help");
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
                    break;
            }
        }
    }
}
