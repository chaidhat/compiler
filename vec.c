#include <stdarg.h>
#include <math.h>
#include "mcc.h"


static bool str (char *buffer, int destSz, int *idest, char *src)
{
    int isrc = 0;
    do
    {
        buffer[*idest] = src[isrc];
        if (++*idest > destSz)
            return true; // error, buffer overflow
    } while (src[++isrc] != '\0');
    return false;
}


void appendChild (Tree *parent, Tree child)
{
    if (parent->noChild == 0)
    {
        parent->children = (Tree *)malloc(sizeof(Tree)); // new children of 1
    }
    else
    {
        // for more than one item in array
        // copy items into treeBuffer, allocate a new space on the heap and then on parent->children
        Tree *treeBuffer = (Tree *)malloc((parent->noChild) * sizeof(Tree));
        for (int i = 0; i < parent->noChild; i++)
            treeBuffer[i] = parent->children[i];


        free(parent->children);
        parent->children = (Tree *)malloc((parent->noChild + 1) * sizeof(Tree));

        for (int i = 0; i < parent->noChild + 1; i++)
            parent->children[i] = treeBuffer[i];
    }
    parent->children[parent->noChild] = child;
    parent->noChild++;
}

bool deleteChild (Tree *parent, char id[128])
{
    int i = 0;
    while (i < parent->noChild && strcmp(parent->children[i].id, id) != 0)
        i++;

    if (i == parent->noChild)
        return false; // cannot find child

    Tree *treeBuffer = (Tree *)malloc((parent->noChild) * sizeof(Tree));
    for (int j = 0; j < parent->noChild; j++)
        treeBuffer[j] = parent->children[j];


    free(parent->children);
    parent->noChild--;

    parent->children = (Tree *)malloc((parent->noChild) * sizeof(Tree));

    int k = 0;
    for (int j = 0; j < parent->noChild; j++)
    {
        if (j == i)
            k++;
        parent->children[j] = treeBuffer[j + k];
    }
    

    return true;
}

void logTree (Tree *t)
{
    mccLog("log of tree  %s", t->id);
    for (int i = 0; i < t->noChild; i++)
        mccLog("\t(%d/%d) %s", i, t->noChild, t->children[i].id);

    mccLog("end log tree %s", t->id);
}


int mccstrtod (char *str)
{
    int num = 0;
    int i = 0;
    while (true)
    {
        if (str[i] == '\0')
            break;
        char n = 0;
        switch (str[i])
        {
            case '0':
                n = 0;  
                break;
            case '1':
                n = 1;
                break;
            case '2':
                n = 2;
                break;
            case '3':
                n = 3;
                break;
            case '4':
                n = 4;
                break;
            case '5':
                n = 5;
                break;
            case '6':
                n = 6;
                break;
            case '7':
                n = 7;
                break;
            case '8':
                n = 8;
                break;
            case '9':
                n = 9;
                break;
            default:
                n = -1;
                break;
        }
        int e = 1;
        for (int j = 0; j < i; j++)
            e *= 10;
        num += e * n;
        i++;
    }
    return num;
}

char *mccdtostr (int in)
{
    printf("aaaa\n");
    int i = 0;
    char str[128];
    char *strout = malloc(sizeof (char[128]));
    if (in == 0)
        return "0";
    do
    {
        char n = '0';
        switch (in % 10)
        {
            case 0:
                n = '0';  
                break;
            case 1:
                n = '1';
                break;
            case 2:
                n = '2';
                break;
            case 3:
                n = '3';
                break;
            case 4:
                n = '4';
                break;
            case 5:
                n = '5';
                break;
            case 6:
                n = '6';
                break;
            case 8:
                n = '8';
                break;
            case 9:
                n = '9';
                break;
            default:
                printf("error %d\n", in % 10);
                break;
        }
        str[i++] = n;

        in = floor(in/10);
    }
    while (in != 0);
    for (int j = 0; j < i; j++)
        strout[j] = str[i - j - 1];

    strout[i] = '\0';
    return strout;
}

bool mccstr (char *dest, int destSz, char *format, ... )
{
    int isrc = 0, idest = 0;
    char buffer[destSz];

    va_list args;
    va_start(args, format);

    char *src = format;
    do
    {
        if (src[isrc] == '%')
        {
            switch (src[++isrc])
            {
                case 'd':
                    if (str(buffer, destSz, &idest, mccdtostr(va_arg(args, int))))
                        return true; // error, buffer overflow
                    break;
                case 's':
                    if (str(buffer, destSz, &idest, va_arg(args, char*)))
                        return true; // error, buffer overflow
                    break;
                default:
                    return true; // error, unsupported type
                    break;
            }
        }
        else
        {
            buffer[idest] = src[isrc];
            if (++idest > destSz)
                return 1; // error, buffer overflow
        }
    } while (src[isrc++] != '\0');
    va_end(args);
    for (int j = 0; j < idest; j++)
        dest[j] = buffer[j];
    return 0; // success
}
