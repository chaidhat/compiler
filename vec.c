#include "mcc.h"


Tree *crtTree (char *id)
{
    static Tree t;
    strcpy(t.id, id);
    t.noChild = 0;
    return &t;
}
void appendTree (Tree *parent, Tree *child)
{
    if (parent->noChild == 0)
    {
        parent->children = (Tree **)malloc(sizeof(Tree*)); // new children of 1
    }
    else
    {
        // for more than one item in array
        // copy items into treeBuffer, allocate a new space on the heap and then on parent->children
        Tree **treeBuffer = (Tree **)malloc((parent->noChild) * sizeof(Tree*));
        for (int i = 0; i < parent->noChild; i++)
            treeBuffer[i] = parent->children[i];


        free(parent->children);
        parent->children = (Tree **)malloc((parent->noChild + 1) * sizeof(Tree*));

        for (int i = 0; i < parent->noChild + 1; i++)
            parent->children[i] = treeBuffer[i];
    }
    parent->children[parent->noChild] = child;
    parent->noChild++;
}

bool deleteTree (Tree *parent, char id[128])
{
    int i = 0;
    while (i < parent->noChild && strcmp(parent->children[i]->id, id) != 0)
    {
        printf("%d\n", i);
        i++;
    }

    if (i == parent->noChild)
        return false; // cannot find child

    Tree **treeBuffer = (Tree **)malloc((parent->noChild) * sizeof(Tree*));
    for (int j = 0; j < parent->noChild; j++)
        treeBuffer[j] = parent->children[j];


    free(parent->children);
    parent->noChild--;

    parent->children = (Tree **)malloc((parent->noChild) * sizeof(Tree*));

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
        mccLog("\t%d %s", i, t->children[i]->id);

    mccLog("end log tree %s", t->id);
}
