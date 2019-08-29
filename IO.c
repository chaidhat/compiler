#include <stdio.h>
#include <stdlib.h>


char dataBuffer[128];
FILE* fp;
char ch;

// reads files
void IOread (char *filename)
{
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        abort();
    }

    printf("The contents of %s file are:\n", filename);

    int i = 0;
    while((ch = fgetc(fp)) != EOF)
    {
        printf("%c", ch);
        dataBuffer[i] = ch;
        i++;
    }

    fclose(fp);
}

