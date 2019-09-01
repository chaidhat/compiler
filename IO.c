#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "bcc.h"

FILE* fp;
char ch;

// thank you to https://www.programiz.com/c-programming/c-file-input-output 
// for the write/read code

// reads files
void IoRead (char *filename)
{
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        IoErr("Read error while opening the file %s", filename);
        IoExit(1, __LINE__);
    }

    int i = 0;
    while((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
        dataBuffer[i] = ch;
        i++;
    }

    fclose(fp);
}

// write files
void IoWrite (char *toFilename)
{
    fp = fopen(toFilename, "w"); // write mode

    if(fp == NULL)
    {
        IoErr("Write error while opening the file %s", toFilename);
        IoExit(1, __LINE__);
    }

    fprintf(fp,"%s",dataBuffer);
    fclose(fp);
}

void IoInp (int argc, char* argv[])
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
                "--version",
                "-m",
                "-o", 
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
                    printf("\033[1;30mv0.1 by Chaidhat Chaimongkol on %s %s\n\033[m", __DATE__, __TIME__);
                    break;
                case 1:
                    mode = -1;
                    if (line != argc - 1)
                    {
                        if (strncmp(argv[line + 1],"x",strlen("x")) == 0)
                            mode = 0;

                        if (strncmp(argv[line + 1],"d",strlen("d")) == 0)
                            mode = 1;

                        if (strncmp(argv[line + 1],"r",strlen("r")) == 0)
                            mode = 2;
                    }

                    if (mode == -1)
                    {
                        printf("no mode exists.\n-m x   compiler debugger\n-m d   debug mode\n-m r   release mode\n");
                        IoExit(1, __LINE__);
                    }
                    line++;
                    break;
                case 2:
                    strcpy(outFilepath, argv[line + 1]);
                    line++;
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
        printf("usage: bcc [--version] [-m<mode>] [-o<outpath>] <inputpath>\n");
        IoExit(1, __LINE__);
    }
}

void IoPrint (char* suffix, char* format, va_list args )
{
    char buf[256];
    snprintf(buf, sizeof buf, "bcc: %s %s\n", suffix, format);
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
    }
    exit(0);
}
