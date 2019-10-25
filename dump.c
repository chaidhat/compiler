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
        case IT_Call:
            strcpy(sType, "CALL");
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
        case IT_Strct:
            strcpy(sType, "STRUCT");
            break;
        case IT_Unin:
            strcpy(sType, "UNION");
            break;
        case IT_Deref:
            strcpy(sType, "DEREF");
            break;
        case IT_Ref:
            strcpy(sType, "REF");
            break;
        case IT_Cond:
            strcpy(sType, "COND");
            break;
        case IT_Ctrl:
            strcpy(sType, "CTRL");
            break;
        case IT_Scope:
            strcpy(sType, "SCOPE");
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
        default:
            strcpy(sType, "CUSTOM ");
            strcat(sType, mccDtostr(type));
            break;
    }
    return sType;
}

static void dumpInst (Tree *tree)
{
    up();
    //print("id: %s", tree->id);
    print("type: %s", ITtostr(tree->type));

    if (tree->id[0] == '\0') // first item could be blank
        return;
    switch (tree->type)
    {
        case IT_Var:
            print("varName: %s", tree->Inst.var.varName);
            print("varType: %s", LTtostr(tree->Inst.var.varType));
            print("isPtr: %d", tree->Inst.var.isPtr);
            print("isStatic: %d", tree->Inst.var.isStatic);
            print("isArray: %d", tree->Inst.var.isArray);
            if (tree->Inst.var.isArray)
            {
                print("arrayLength:");
                up();
                    dumpTree(tree->Inst.var.arrayLength);
                down();
            }
            break;
        case IT_Func:
            print("funcType: %s", LTtostr(tree->Inst.func.retType));
            print("isPtr: %d", tree->Inst.func.isPtr);
            print("isStatic: %d", tree->Inst.func.isStatic);
            print("funcName: %s", tree->Inst.func.funcName);
            if (tree->Inst.func.parameters->noChild > 0)
            {
                print("");
                print("parameters:");
                up();
                    for (int i = 0; i < tree->Inst.func.parameters->noChild; i++)
                        dumpTree(&tree->Inst.func.parameters->children[i]);
                down();
            }
            if (tree->Inst.func.scope->noChild > 0)
            {
                print("");
                print("scope:");
                up();
                    for (int i = 0; i < tree->Inst.func.scope->noChild; i++)
                        dumpTree(&tree->Inst.func.scope->children[i]);
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
            if (tree->Inst.id.nested->noChild > 0)
            {
                print("");
                print("nested:");
                up();
                    for (int i = 0; i < tree->Inst.id.nested->noChild; i++)
                        dumpTree(&tree->Inst.id.nested->children[i]);
                down();
            }
            break;
        case IT_Ret:
            print("exprsn:");
            up();
                dumpTree(tree->Inst.ret.exprsn);
            down();
            break;
        case IT_Call:
            print("funcName: %s", tree->Inst.call.funcName);
            if (tree->Inst.call.args->noChild > 0)
            {
                print("");
                print("args:");
                up();
                    for (int i = 0; i < tree->Inst.call.args->noChild; i++)
                        dumpTree(&tree->Inst.call.args->children[i]);
                down();
            }
            break;
        case IT_Assign:
            print("varName:");
            up();
                dumpTree(tree->Inst.assign.varName);
            down();
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
            if (!tree->Inst.binary.single)
            {
                print("");
                print("right:");
                up();
                    dumpTree(tree->Inst.binary.right);
                down();
            }
            break;
        case IT_Strct:
            print("strctName: %s", tree->Inst.strct.strctName);
            print("noChild: %d", tree->Inst.strct.decls->noChild);
            print("");
            print("decls:");
            up();
                dumpTree(tree->Inst.strct.decls);
            down();
            down();
            break;
        case IT_Unin:
            print("uninName: %s", tree->Inst.unin.uninName);
            print("noChild: %d", tree->Inst.unin.decls->noChild);
            print("");
            print("decls:");
            up();
                dumpTree(tree->Inst.unin.decls);
            down();
            down();
            break;
        case IT_Deref:
            print("exprsn:");
            up();
                dumpTree(tree->Inst.deref.exprsn);
            down();
            break;
        case IT_Ref:
            print("ptr:");
            up();
                dumpTree(tree->Inst.ref.ptr);
            down();
            break;
        case IT_Cond:
            print("exprsn:");
            up();
                dumpTree(tree->Inst.cond.exprsn);
            down();
            print("");
            print("scope:");
            up();
                for (int i = 0; i < tree->Inst.cond.scope->noChild; i++)
                    dumpTree(&tree->Inst.cond.scope->children[i]);
            down();
            break;
        case IT_Ctrl:
            print("exprsn:");
            up();
                dumpTree(tree->Inst.cond.exprsn);
            down();
            print("");
            print("scope:");
            up();
                for (int i = 0; i < tree->Inst.ctrl.scope->noChild; i++)
                    dumpTree(&tree->Inst.ctrl.scope->children[i]);
            down();
            break;
        case IT_Scope:
            break;
        default:
            mccErr("AST DUMP ERR %s in dump.c", __LINE__);
            break;
    }
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
    down();
    inpWrite(outFilepath, "-dump-ast.txt");
}

void dumpPp ()
{
    inpWrite(outFilepath, "-pp.mc");
    mccExit(0, __LINE__);
}
