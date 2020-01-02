#include "mcc.h"
static char datBuf[DB_SIZE];

FILE* fp;

// thank you to https://www.programiz.com/c-programming/c-file-input-output 
// for the write/read code

// reads files
void inpOpen (char *filename)
{
    if (strcmp(filename, "$") == 0)
    {
        mccErr("no input file.\n ./mcc -h for help");
        mccExit(2);
    }
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        mccErr("Read error while opening the file %s", filename);
        mccExit(1);
    }

    if (strcmp(inFilepath, filename) != 0)
        strcpy(inFilepath, filename);
    inp();
    inpPos.h = -1;
    inpPos.total = 1;
    inpPos.line = 0;
    resetEOF();

}

void inpClose ()
{
    fclose(fp);
}

// write files
void inpWrite (char *toFilename, char *extn)
{
    char filename[128];
    if (!isChangeFilepath)
    {
        strcpy(filename, toFilename);
        strcat(filename, extn);
    }
    else
        strcpy(filename, toFilename);

    fp = fopen(filename, "w"); // write mode

    if(fp == NULL)
    {
        mccErr("Write error while opening the file %s", filename);
        mccExit(1);
    }

    fprintf(fp,"%s",datBuf);
    fclose(fp);
}
 
void inpPush (char *inDataBuffer)
{
    strcat(datBuf, inDataBuffer);
}

void inpPop ()
{
    datBuf[0] = '\0';
}

void inpGoto(Pos pos)
{
    for (int i = 0; i < pos.total - 1; i++)
        inp();
}

char inp ()
{
    inpT = inpN; 
    inpPos.h++;
    inpPos.total++;
    if ((inpN = fgetc(fp)) == EOF)
        inpN = '\0';
    if (inpT == '\n')
    {
        inpPos.line++;
        inpPos.h = 0;
    }
    if (inpT == '\\' && inpN == '\n')
        inp();
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

void inpGetFilename (char *outFilename, int outFilenameSz)
{
    int i;
    for (i = 0; i < outFilenameSz; i++)
    {
        if (outFilepath[i] == '\0')
            break;
    }
    while (i > 0)
    {
        i--;
        if (outFilepath[i] == '/' || outFilepath[i] == '\\')
            break;
    }
    i++;
    for (int j = 0; i < outFilenameSz; j++)
    {
        outFilename[j] = outFilepath[i];
        if (outFilepath[i] == '\0')
            break;
        i++;
    }
}
