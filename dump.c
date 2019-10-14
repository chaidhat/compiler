#include <stdarg.h>
#include "mcc.h"

static int scope = 0;
static void print (char* format, ... );
static void up ();
static void down ();
static void dumpInst (Tree *tree);
static void dumpTree (Tree *tree);

static char *ITtostr (enum InstType type);

static void print (char* fmt, ... )
{
    char buf[256];
    printf("mcc fdast: ");
    for (int i = 0; i < scope; i++)
    {
        printf("| ");
        inpPush("| ");
    }
    va_list args;
    va_start(args, fmt);
 
    while (*fmt != '\0') {
        if (*fmt == '%')
        {
            fmt++;
            if (*fmt == 'd') {
                int i = va_arg(args, int);
                printf("%s", mccDtostr(i));
                inpPush(mccDtostr(i));
            } else if (*fmt == 's') {
                char *s = va_arg(args, char *);
                printf("%s", s);
                inpPush(s);
            } else
            {
                mccErr("AST DUMP ERR %s in dump.c", __LINE__);
            }
        }
        else
        {
            printf("%c", *fmt);
            char st[2];
            st[0] = *fmt;
            st[1] = '\0';
            inpPush(st);
        }
        fmt++;
    }
    printf("\n");
    inpPush("\n");
    va_end(args);
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

static char *LTtostr (enum LitType type)
{
    static char sType[128];
    switch (type)
    {
        case LT_CHAR:
            strcpy(sType, "BYTE");
            break;
        case LT_INT:
            strcpy(sType, "INT");
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
            print("varType: %s", LTtostr(tree->Inst.var.varType));
            print("isPtr: %d", tree->Inst.var.isPtr);
            print("isStatic: %d", tree->Inst.var.isStatic);
            print("varName: %s", tree->Inst.var.varName);
            break;
        case IT_Func:
            print("funcType: %s", LTtostr(tree->Inst.func.retType));
            print("isPtr: %d", tree->Inst.func.isPtr);
            print("isStatic: %d", tree->Inst.func.isStatic);
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
            print("type: %s", LTtostr(tree->Inst.lit.type));
            if (tree->Inst.lit.type == LT_INT)
                print("val: %d", tree->Inst.lit.val.tInt);
            else
                mccErr("val: CHAR NOT PROG"); // TODO: char
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
            mccErr("AST DUMP ERR %s in dump.c", __LINE__);
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
    print("%s AST:", inFilepath);
    up();
    dumpTree(tree);
    down();
    char outFilepathS[128];
    strcpy(outFilepathS, outFilepath);
    strcat(outFilepathS, ".mins");
    inpWrite(outFilepathS);
}

