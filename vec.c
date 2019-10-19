#include "mcc.h"


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


int mccStrtod (char *str)
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

char *mccDtostr (int in)
{
    double sIn = in;
    char *str = malloc(sizeof(char[128]));
    if (sIn == 0)
        return "0";
    for (int i = 0; sIn >= 1; i++)
    {
        sIn /= 10;
        char n = '0';
        switch (in)
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
                n = 'e';
                break;
        }
        str[i] = n;
        str[i + 1] = '\0';
    }
    return str;
}
