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
    printf("mcc fd: ");
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
                printf("%s", mccdtostr(i));
                inpPush(mccdtostr(i));
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
            strcat(sType, mccdtostr(type));
            break;
    }
    return sType;
}

static void dumpInst (Tree *tree)
{
    up();
    //print("id: %s", tree->id);
    print("type: %s", ITtostr(tree->ast.type));

    if (tree->id[0] == '\0') // first item could be blank
        return;
    switch (tree->ast.type)
    {
        case IT_Var:
            print("varName: %s", tree->ast.var.varName);
            print("varType: %s", LTtostr(tree->ast.var.varType));
            print("isPtr: %d", tree->ast.var.isPtr);
            print("isStatic: %d", tree->ast.var.isStatic);
            print("isArray: %d", tree->ast.var.isArray);
            if (tree->ast.var.isArray)
            {
                print("arrayLength:");
                up();
                    dumpTree(tree->ast.var.arrayLength);
                down();
            }
            break;
        case IT_Func:
            print("funcType: %s", LTtostr(tree->ast.func.retType));
            print("isPtr: %d", tree->ast.func.isPtr);
            print("isStatic: %d", tree->ast.func.isStatic);
            print("funcName: %s", tree->ast.func.funcName);
            if (tree->ast.func.parameters->childrenSz > 0)
            {
                print("");
                print("parameters:");
                up();
                    for (int i = 0; i < tree->ast.func.parameters->childrenSz; i++)
                        dumpTree(&tree->ast.func.parameters->children[i]);
                down();
            }
            if (tree->ast.func.scope->childrenSz > 0)
            {
                print("");
                print("scope:");
                up();
                    for (int i = 0; i < tree->ast.func.scope->childrenSz; i++)
                        dumpTree(&tree->ast.func.scope->children[i]);
                down();
            }
            break;
        case IT_Lit:
            print("type: %s", LTtostr(tree->ast.lit.type));
            if (tree->ast.lit.type == LT_INT)
                print("val: %d", tree->ast.lit.val.tInt);
            else
                mccErr("val: CHAR NOT PROG"); // TODO: support LT_CAHR
            break;
        case IT_Id:
            print("isPtr: %d", tree->ast.id.isPtr);
            print("varName: %s", tree->ast.id.varName);
            if (tree->ast.id.nested->childrenSz > 0)
            {
                print("");
                print("nested:");
                up();
                    for (int i = 0; i < tree->ast.id.nested->childrenSz; i++)
                        dumpTree(&tree->ast.id.nested->children[i]);
                down();
            }
            break;
        case IT_Ret:
            print("exprsn:");
            up();
                dumpTree(tree->ast.ret.exprsn);
            down();
            break;
        case IT_Call:
            print("funcName: %s", tree->ast.call.funcName);
            if (tree->ast.call.args->childrenSz > 0)
            {
                print("");
                print("args:");
                up();
                    for (int i = 0; i < tree->ast.call.args->childrenSz; i++)
                        dumpTree(&tree->ast.call.args->children[i]);
                down();
            }
            break;
        case IT_Assign:
            print("varName:");
            up();
                dumpTree(tree->ast.assign.varName);
            down();
            print("");
            print("exprsn:");
            up();
                dumpTree(tree->ast.assign.exprsn);
            down();
            break;
        case IT_Binary:
            print("stub: %d", tree->ast.binary.stub);
            print("single: %d", tree->ast.binary.single);
            print("op (id): %s", tree->ast.binary.op.id);
            print("");
            print("left:");
            up();
                dumpTree(tree->ast.binary.left);
            down();
            if (!tree->ast.binary.single)
            {
                print("");
                print("right:");
                up();
                    dumpTree(tree->ast.binary.right);
                down();
            }
            break;
        case IT_Strct:
            print("strctName: %s", tree->ast.strct.strctName);
            print("childrenSz: %d", tree->ast.strct.decls->childrenSz);
            print("");
            print("decls:");
            up();
                dumpTree(tree->ast.strct.decls);
            down();
            down();
            break;
        case IT_Unin:
            print("uninName: %s", tree->ast.unin.uninName);
            print("childrenSz: %d", tree->ast.unin.decls->childrenSz);
            print("");
            print("decls:");
            up();
                dumpTree(tree->ast.unin.decls);
            down();
            down();
            break;
        case IT_Deref:
            print("exprsn:");
            up();
                dumpTree(tree->ast.deref.exprsn);
            down();
            break;
        case IT_Ref:
            print("ptr:");
            up();
                dumpTree(tree->ast.ref.ptr);
            down();
            break;
        case IT_Cond:
            print("exprsn:");
            up();
                dumpTree(tree->ast.cond.exprsn);
            down();
            print("");
            print("scope:");
            up();
                for (int i = 0; i < tree->ast.cond.scope->childrenSz; i++)
                    dumpTree(&tree->ast.cond.scope->children[i]);
            down();
            break;
        case IT_Ctrl:
            print("exprsn:");
            up();
                dumpTree(tree->ast.cond.exprsn);
            down();
            print("");
            print("scope:");
            up();
                for (int i = 0; i < tree->ast.ctrl.scope->childrenSz; i++)
                    dumpTree(&tree->ast.ctrl.scope->children[i]);
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
    for (int i = 0; i < tree->childrenSz; i++)
        dumpTree(&tree->children[i]);
}

void dumpAst (Tree *ast)
{
    mccWarn("dumping AST...");
    print("%s AST:", inFilepath);
    up();
    for (int i = 0; i < ast->childrenSz; i++)
        dumpTree(&ast->children[i]);
    down();
    inpWrite(outFilepath, ".md");
}

void dumpPp ()
{
    inpWrite(outFilepath, ".mi");
    mccExit(0);
}
