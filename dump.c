#include <stdarg.h>
#include "mcc.h"

static int scope = 0;
static void print (char* format, ... );
static void up ();
static void down ();
static void dumpInst (Tree *tree);
static void dumpTree (Tree *tree);

static char *ITtostr (enum InstType type);

static void print (char* format, ... )
{
    va_list args;
    va_start (args, format);
    char buf[256];
    printf("mcc fdast: ");
    for (int i = 0; i < scope; i++)
        printf("| ");
    snprintf(buf, sizeof buf, "%s\n", format);
    vprintf (buf, args);
}

static void up ()
{
    print("{");
    scope++;
}

static void down ()
{
    scope--;
    print("},");
}

static char *ITtostr (enum InstType type)
{
    static char sType[128];
    switch (type)
    {
        case IT_Var:
            strcpy(sType, "VAR");
            break;
        case IT_Func:
            strcpy(sType, "FUNC");
            break;
        case IT_Ret:
            strcpy(sType, "RET");
            break;
        case IT_Lit:
            strcpy(sType, "LIT");
            break;
        case IT_Id:
            strcpy(sType, "ID");
            break;
        case IT_Assign:
            strcpy(sType, "ASSIGN");
            break;
        case IT_Binary:
            strcpy(sType, "BINARY");
            break;
        default:
            printf("UNKNOWN %d\n", type);
            strcpy(sType, "UNKNOWN");
            break;
    }
    return sType;
}

static void dumpInst (Tree *tree)
{
    up();
    print("id: %s", tree->id);
    print("type: %s", ITtostr(tree->type));
    print("Inst:");

    up();
    switch (tree->type)
    {
        case IT_Var:
            print("varType: %d", tree->Inst.var.varType);
            print("isPtr: %d", tree->Inst.var.isPtr);
            print("varName: %s", tree->Inst.var.varName);
            break;
        case IT_Func:
            print("funcType: %d", tree->Inst.func.retType);
            print("isPtr: %d", tree->Inst.func.isPtr);
            print("funcName: %s", tree->Inst.func.funcName);
            print("");
            print("noParameters: %d", tree->Inst.func.noParameters);
            if (tree->Inst.func.noParameters > 0)
            {
                print("parameters:");
                up();
                    for (int i = 0; i < tree->Inst.func.noParameters; i++)
                        dumpTree(&tree->Inst.func.parameters->children[i]);
                down();
            }
            print("");
            print("noScope: %d", tree->Inst.func.noParameters);
            if (tree->Inst.func.noScope > 0)
            {
                print("scope:");
                up();

                down();
            }
            break;
        case IT_Lit:
            print("type: %d", tree->Inst.lit.type);
            if (tree->Inst.lit.type == LT_INT)
                print("val: %d", tree->Inst.lit.val.tInt);
            else
                print("val: CHAR NOT PROG");
            break;
        case IT_Id:
            print("isPtr: %d", tree->Inst.id.isPtr);
            print("varName: %s", tree->Inst.id.varName);
            break;
        case IT_Assign:
            print("varName: %s", tree->Inst.assign.varName);
            print("");
            print("exprsn:");
            up();
                dumpTree(tree->Inst.assign.exprsn);
            down();
            break;
        case IT_Binary:
            print("stub: %d", tree->Inst.binary.stub);
            print("single: %d", tree->Inst.binary.single);
            print("op (id): %s", tree->Inst.binary.op.id);
            print("");
            print("left:");
            up();
                dumpTree(tree->Inst.binary.left);
            down();
            print("");
            if (!tree->Inst.binary.single)
            {
                print("right:");
                up();
                    dumpTree(tree->Inst.binary.right);
                down();
            }
            break;
        default:
            print("ERR");
            break;
    }
    down();
    down();
}

static void dumpTree (Tree *tree)
{
    dumpInst(tree);
    for (int i = 0; i < tree->noChild; i++)
        dumpTree(&tree->children[i]);
}

void dumpAst (Tree *tree)
{
    mccWarn("dumping AST...");
    logTree(tree);
    dumpTree(tree);
}

