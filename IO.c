#include <stdarg.h>
#include "btcc.h"

FILE* fp;

// thank you to https://www.programiz.com/c-programming/c-file-input-output 
// for the write/read code

// reads files
void IoOpen (char *filename)
{
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        IoErr("Read error while opening the file %s", filename);
        IoExit(1, __LINE__);
    }

    inp();
    inpPos.h = -1;
    inpPos.line = 0;

}
void IoClose ()
{
    fclose(fp);
}

char inp ()
{
    inpT = inpN; 
    inpPos.h++;
    if ((inpN = fgetc(fp)) == EOF)
        inpN = '\0';
    if (inpT == '\n')
        inpPos.line++;
    return inpT;
}

bool inpCT (char expect)
{
    if (inpT == expect)
        return true;
    return false;
}

bool inpCN (char expect)
{
    if (inpN == expect)
        return true;
    return false;
}


// write files
void IoOutput (char *toFilename)
{
    fp = fopen(toFilename, "w"); // write mode

    if(fp == NULL)
    {
        IoErr("Write error while opening the file %s", toFilename);
        IoExit(1, __LINE__);
    }

    fclose(fp);
}

void IoUI (int argc, char* argv[])
{
    strcpy(inFilepath, "main.btc");
    strcpy(outFilepath, "$");
    mode = 0; // default mode (compiler debug)
    if (argc > 1)
    {
        for (int line = 1; line < argc; line++)
        {
            char* args[] =
            {
                "-v",
                "-b",
                "-o",
                "-r",
                "-g",
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
                    printf("\033[1;30mBit-C Compiler created by Chaidhat Chaimongkol\n%s %s\n\033[m", __DATE__, __TIME__);
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
                    strcat(predefPP, "#include ");
                    strcat(predefPP, argv[line + 1]);
                    strcat(predefPP, ";");
                    line++;
                    break;
                case 7:
                    strcat(predefPP, "#define ");
                    int c = 0;
                    while (argv[line + 1][c] != '\0')
                    {
                        c++;
                        if (argv[line + 1][c] == '=')
                            argv[line + 1][c] = ' '; 
                    }
                    strcat(predefPP, argv[line + 1]);
                    strcat(predefPP, ";");
                    printf("%s\n", predefPP);
                    line++;
                    break;
                case 8:
                    strcat(predefPP, "#undef ");
                    strcat(predefPP, argv[line + 1]);
                    strcat(predefPP, ";");
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
                    printf("\033[1;30mBit-C Compiler created by Chaidhat Chaimongkol\n%s %s\n\n\033[m", __DATE__, __TIME__);
                    printf("usage: btcc [arg1 arg2 ...] [-h] [-g] <inpath1 inpath2 ...>\n\n" 
                    "args:\n"
                    "   -v                   display version info\n"
                    "   -b                   display benchmarking stats\n"
                    "   -o <path>            write output to <path>\n"
                    "   -r <args ...>        run output\n"
                    "   -g                   verbose compiler debugger\n"
                    "   -w                   supress all warnings\n"
                    "\n"
                    "   -I <dir>             add include path <dir>\n"
                    "   -D <macro>           predefine <macro>\n"
                    "   -D <macro>[=val]     set <macro> to [val]\n"
                    "   -U <macro>           undefine <macro>\n"
                    "   -E                   stop after preproccessing\n"
                    "\n"
                    "   -S                   stop after parsing\n"
                    "   -c                   do not link. Generate .o\n"
                    "\n"
                    "   -h                   display this help\n"
                    "\n"
                    );
                    IoExit(2, __LINE__);
                    break;
                default:
                    strcpy(inFilepath, argv[line]);
                    char btcName[100];
                    int i = 0;
                    int j = 0;
                    while (argv[line][i] != '.')
                        i++;
                    if (argv[line][i + 1] != 'b' || argv[line][i + 2] != 't' || argv[line][i + 3] != 'c')
                    {
                        IoErr("Unexpected file type (expected .btc)");
                        IoExit(1, __LINE__);
                    }
                    while (i > 0)
                    {
                        i--;
                        j++;
                    }
                    while (j > 0)
                    {
                        j--;
                        btcName[j] = argv[line][i + j];
                    }

                    if (strncmp(outFilepath,"$",strlen("$")) == 0)
                        strcpy(outFilepath, btcName);
                        strcat(outFilepath, ".o");
                    break;
            }
        }
    }
    else
    {
        IoErr("no arguments given.\n ./btcc -h for help");
        IoExit(2, __LINE__);
    }
}

void IoPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "btcc: %s %s\n", suffix, format);
  vprintf (buf, args);
}

void IoLog (char* format, ... )
{
    va_list args;
    va_start (args, format);
    IoPrint("", format, args);
    va_end (args);
}
void IoWarn (char* format, ... )
{
    va_list args;
    va_start (args, format);
    IoPrint("\033[1;33mWARNING:\033[m", format, args);
    va_end (args);
}
void IoErr (char* format, ... )
{
    va_list args;
    va_start (args, format);
    IoPrint("\033[1;31mERROR:\033[m", format, args);
    va_end (args);
}

void IoExit (int code, int debugLine)
{
    switch (code)
    {
        case 0:
            IoLog("Terminated sucessfully.");
            break;
        case 1:
            IoLog("Terminated UNsucessfully.");
            if (mode == 0)
                IoLog("Called from line %d", debugLine);
            break;
        case 2:
            if (mode == 0)
                IoLog("Terminated calm. Called from line %d", debugLine);
            break;
    }
    exit(0);
}
