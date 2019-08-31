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
        perror("Error while opening the file.\n");
        exit(1);
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
    fp = fopen(toFilename,"w");

    if(fp == NULL)
    {
       perror("Error while opening the file.\n");
       exit(1);             
    }

    fprintf(fp,"%s",dataBuffer);
    fclose(fp);
}

int IoInp (int argc, char* argv[], char** inFilepath, char** outFilepath)
{
    mode = 1;
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
                    printf("v0.1 by Chaidhat Chaimongkol\n");
                    return 0;

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
                        return 0;
                    }
                    line++;
                    break;
                case 2:
                    *outFilepath = argv[line + 1];
                    line++;
                    break;
                default:
                    *inFilepath = argv[line];
                    char btcName[100];
                    int i = 0;
                    int j = 0;
                    while (argv[line][i] != '.')
                        i++;
                    if (argv[line][i + 1] != 'b' || argv[line][i + 2] != 't' || argv[line][i + 3] != 'c')
                        IoErr("Unrecognised file type (expected .btc)");
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

                   // IoL("h%s\n", btcName);
                    if (strncmp(*outFilepath,"$",strlen("$")) == 0)
                        *outFilepath = btcName;
                        strcat(*outFilepath, ".o");
                    break;
            }
        }
    }
    else
    {
        printf("usage: bcc [--version] [-m<mode>] [-o<outpath>] <inputpath>\n");
        return 0;
    }
    return 1;
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
