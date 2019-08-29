#include <stdio.h>
#include <stdlib.h>


char dataBuffer[128];
FILE* fp;
char ch;
// thank you to https://www.programiz.com/c-programming/c-file-input-output

// reads files
void IOread (char *filename)
{
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(1);
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
void IOwrite (char *toFilename)
{
    char *ch2 = "a";
    // use appropriate location if you are using MacOS or Linux
    fp = fopen(toFilename,"w");
    if(fp == NULL)
    {
       perror("Error while opening the file.\n");
       exit(1);             
    }
    fprintf(fp,"%s",dataBuffer);
    fclose(fp);
}
char* getDataBuffer(){
    return dataBuffer;
}
void setDataBuffer(char *inData)
{
    for (int i = 0; i < 128; i++)
    {
        dataBuffer[i] = inData[i];
    }
}


