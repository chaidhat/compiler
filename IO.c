#include <stdio.h>
#include <stdlib.h>
#include "bcc.h"


FILE* fp;
char ch;

// thank you to https://www.programiz.com/c-programming/c-file-input-output 
// for the write/read code

// reads files
void IOread (char *filename)
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
void IOwrite (char *toFilename)
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
