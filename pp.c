#include "mcc.h"

char macros[128][128];
int macrosval[128];
int macroNo = 0;

static void logMacro (char *name, int value);
static int getMacro (char *name);

static void logMacro (char *name, int value)
{
    strcpy(macros[macroNo], name);
    macrosval[macroNo] = value;
    macroNo++;
    return;
}

static int getMacro (char *name)
{
    char cname[128];
    int i = 0;
    while (strcmp(cname, name) != 0 && i < 128)
    {
        strcpy(cname, macros[i++]);
    }
    if (i == 128)
        return 0;
    return macrosval[i];
}


void readDefine ()
{
    printf("define %s\n", lex().id);
    logMacro(tokT.id, 1);
    return;
}
void readIf ()
{
    printf("if %s\n", lex().id);
    printf("%d\n", getMacro(tokT.id));
    return;
}
