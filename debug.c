#include <stdarg.h>
#include <windows.h>
#include <conio.h>
#include "mcc.h"

// http://www.codebind.com/cprogramming/get-current-directory-using-c-program/
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #define OS "Windows" 
#elif __APPLE__
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #define OS "Mac OS X" 
#else
    #error MCC Does not support/recognise this OS
#endif


enum MsgType
{
    Log,
    Warn,
    Err,
};

static void colourPrint (int colour, char *msg)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, colour);
    printf("%s", msg);
    SetConsoleTextAttribute(hcon, 7);
}

static void mccPrint (enum MsgType msgType, char* prefix, char* suffix, char* format, va_list args )
{
    printf("mcc: ");
    switch (msgType)
    {
        case Log:
            break;
        case Warn:
            colourPrint(12, "warning: ");
            break;
        case Err:
            colourPrint(12, "error: ");
            break;
    };
    char buf[256];
    snprintf(buf, sizeof buf, "%s\t%d\t%s %s\n", prefix, inpPos.line, format, suffix);
  vprintf (buf, args);
}

static void mccPrintE (char* suffix, char* format, va_list args)
{
    char buf[256];
    snprintf(buf, sizeof buf, "%s: %s at (%d,%d):\n    %s\033[m\n", inFilepath, suffix, inpPos.line + 1, inpPos.h,  format);
    vprintf (buf, args);
}

void debugLog (char *filename, int line, char* format, ... )
{
    char prefix[128];
    strcpy(prefix, filename);
    strcat(prefix, ",");
    strcat(prefix, mccdtostr(line));
    strcat(prefix, ":");
    va_list args;
    va_start (args, format);
    if (mode == 0)
        mccPrint(Log, prefix, "", format, args);
    va_end (args);
}
void debugWarn (char *filename, int line, char* format, ... )
{
    char prefix[128];
    strcpy(prefix, "");
    if (mode == 0)
    {
        strcpy(prefix, filename);
        strcat(prefix, ",");
        strcat(prefix, mccdtostr(line));
        strcat(prefix, ":");
    }
    va_list args;
    va_start (args, format);
    mccPrint(Warn, prefix, "", format, args);
    va_end (args);
}
void debugErr (char *filename, int line, char* format, ... )
{
    char prefix[128];
    strcpy(prefix, "");
    if (mode == 0)
    {
        strcpy(prefix, filename);
        strcat(prefix, ",");
        strcat(prefix, mccdtostr(line));
        strcat(prefix, ":");
    }
    va_list args;
    va_start (args, format);
    mccPrint(Err, prefix, "", format, args);
    va_end (args);
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
        mccExit(1);
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
    mccExit(1);
}

void debugExit (char *filename, int line, int code)
{
    switch (code)
    {
        case 0:
            printf("mcc: Terminated sucessfully.");
            break;
        case 1:
            mccErr("Terminated UNsucessfully.");
            if (mode == 0)
                mccLog("Called from %s %d", filename, line);
            break;
        case 2:
            if (mode == 0)
                mccLog("Terminated calmly. Called from %s %d", filename, line);
            break;
    }
    exit(code);
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
                "-g",
                "-w",
                "-W",
                "-We",
                "-I",
                "-D",
                "-E",
                "-S",
                "-c",
                "-fd",
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
                    printf("MinimalistiC Compiler created by Chaidhat Chaimongkol\n"
                    "https://github.com/Chai112/MinC-Compiler\n"
                    "Compiled on: %s %s\n"
                    "Target: x86 Intel (32 bit) %s\n"
                    "Installed with: gcc %s\n"
                    "Installed dir: %s\n"
                    "\n"
                    , __DATE__, __TIME__, OS, __VERSION__, homedir);
                    mccExit(2);
                    break;
                case 1: // -b
                    doBenchmarking = true; 
                    break; 
                case 2: // -o
                    strcpy(outFilepath, argv[line + 1]);
                    isChangeFilepath = true;
                    line++;
                    break;
                case 3: // -g
                    mode = 0;
                    break;
                case 4: // -w
                    doWarnings = false;
                    break;
                case 5: // -W
                    // TODO
                    line++;
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
                    doLink = false;
                    break;
                case 12: // -fd
                    doDumpAst = true;
                    break;
                case 13: // -h
                    printf("MinimalistiC Compiler created by Chaidhat Chaimongkol\n%s %s\n", __DATE__, __TIME__);
                    printf("https://github.com/Chai112/MinC-Compiler\n\n"
                    "usage: mcc [-h] [-V] [arg1 arg2 ...] <inpath1 inpath2 ...>\n\n" 
                    "mcc only accepts .mc as inpath, dir and include accepts any types\n"
                    "args:\n"
                    "   -V                  display version info\n"
                    "   -b                  display compilation benchmarks\n"
                    "   -o <path>           write output to <path>\n"
                    "\n"
                    "   -g                  enable compiler debugger\n"
                    "   -w                  supress all warnings\n"
                    "   -W <wcode>          supress <wcode> warning\n"
                    "   -We                 treat all warnings as errors\n"
                    "\n"
                    "   -I <dir>            add include path <dir>\n"
                    "   -D <macro> <val>    set <macro> to <val>\n"
                    "   -E                  preprocess only\n"
                    "\n"
                    "   -S                  stop before assembly\n"
                    "   -c                  stop before linking\n"
                    "   -fd                 print AST\n"
                    "\n"
                    "   -h                  display this help\n"
                    "\n"
                    );
                    mccExit(2);
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
                        if (argv[line][0] == '-')
                        {
                            mccErr("Unknown flag!\n ./mcc -h for help");
                            mccExit(1);
                        }
                        mccErr("Unexpected file type (expected .mc)\n ./mcc -h for help");
                        mccExit(1);
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
    strcpy(startFilepath, inFilepath);
}
