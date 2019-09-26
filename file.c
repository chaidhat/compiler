#include "mcc.h"

FILE* fp;

// thank you to https://www.programiz.com/c-programming/c-file-input-output 
// for the write/read code

// reads files
void inpOpen (char *filename)
{
    fp = fopen(filename, "r"); // read mode

    if (fp == NULL)
    {
        mccErr("Read error while opening the file %s", filename);
        mccExit(1, __LINE__);
    }

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
void inpOutput (char *toFilename)
{
    fp = fopen(toFilename, "w"); // write mode

    if(fp == NULL)
    {
        mccErr("Write error while opening the file %s", toFilename);
        mccExit(1, __LINE__);
    }

    fclose(fp);
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
