#include "mcc.h"


Tree *crtTree (char *id)
{
    static Tree t;
    strcpy(t.id, id);
    return &t;
}
void appendTree (Tree *child, Tree *parent)
{
}
