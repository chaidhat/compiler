#include "mcc.h"
#define genfileinserts(str) if( \
        mccstr(dest, destSz, "%s" str "\n", dest)) \
        { mccErr("gen insert error\n"); mccExit(1, __LINE__); }
#define genfileinsertm(str, ... ) if( \
        mccstr(dest, destSz, "%s" str "\n", dest, __VA_ARGS__)) \
        { mccErr("gen insert error\n"); mccExit(1, __LINE__); }
static char datBuf[DB_SIZE];

static void genHeader (char *dest, int destSz);


static void genHeader (char *dest, int destSz)
{
    mccstr(dest, destSz, " \n");
    genfileinserts("# Compiled with Chaidhat Chaimongkol's         ");
    genfileinserts("# MinimalistiC Compiler                        ");
    genfileinserts(" ");
    genfileinsertm("      .file   \"%s%s\"                         ", outFilepath, isChangeFilepath ? " " : ".mc");
    genfileinserts("      .def    ___main; .scl 2; .type 32; .endef");
    genfileinserts("      .text                                    ");
    genfileinserts(" ");
    genfileinserts("# global function declarations                 ");
    genfileinserts("      .globl  _main                            ");
    genfileinserts(" ");
    genfileinserts("# all function declarations                    ");
    genfileinserts("      .def    _main;   .scl 2; .type 32; .endef");
}

static void genInst (Tree *tree)
{
     
    /*print("type: %s", ITtostr(tree->type));

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
                 
                    genTree(tree->Inst.var.arrayLength);
                
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
                 
                    for (int i = 0; i < tree->Inst.func.parameters->noChild; i++)
                        genTree(&tree->Inst.func.parameters->children[i]);
                
            }
            if (tree->Inst.func.scope->noChild > 0)
            {
                print("");
                print("scope:");
                 
                    for (int i = 0; i < tree->Inst.func.scope->noChild; i++)
                        genTree(&tree->Inst.func.scope->children[i]);
                
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
                 
                    for (int i = 0; i < tree->Inst.id.nested->noChild; i++)
                        genTree(&tree->Inst.id.nested->children[i]);
                
            }
            break;
        case IT_Ret:
            print("exprsn:");
             
                genTree(tree->Inst.ret.exprsn);
            
            break;
        case IT_Call:
            print("funcName: %s", tree->Inst.call.funcName);
            if (tree->Inst.call.args->noChild > 0)
            {
                print("");
                print("args:");
                 
                    for (int i = 0; i < tree->Inst.call.args->noChild; i++)
                        genTree(&tree->Inst.call.args->children[i]);
                
            }
            break;
        case IT_Assign:
            print("varName:");
             
                genTree(tree->Inst.assign.varName);
            
            print("");
            print("exprsn:");
             
                genTree(tree->Inst.assign.exprsn);
            
            break;
        case IT_Binary:
            print("stub: %d", tree->Inst.binary.stub);
            print("single: %d", tree->Inst.binary.single);
            print("op (id): %s", tree->Inst.binary.op.id);
            print("");
            print("left:");
             
                genTree(tree->Inst.binary.left);
            
            if (!tree->Inst.binary.single)
            {
                print("");
                print("right:");
                 
                    genTree(tree->Inst.binary.right);
                
            }
            break;
        case IT_Strct:
            print("strctName: %s", tree->Inst.strct.strctName);
            print("noChild: %d", tree->Inst.strct.decls->noChild);
            print("");
            print("decls:");
             
                genTree(tree->Inst.strct.decls);
            
            
            break;
        case IT_Unin:
            print("uninName: %s", tree->Inst.unin.uninName);
            print("noChild: %d", tree->Inst.unin.decls->noChild);
            print("");
            print("decls:");
             
                genTree(tree->Inst.unin.decls);
            
            
            break;
        case IT_Deref:
            print("exprsn:");
            
                genTree(tree->Inst.deref.exprsn);
            
            break;
        case IT_Ref:
            print("ptr:");
            
                genTree(tree->Inst.ref.ptr);
            
            break;
        case IT_Cond:
            print("exprsn:");
            
                genTree(tree->Inst.cond.exprsn);
            
            print("");
            print("scope:");
            
                for (int i = 0; i < tree->Inst.cond.scope->noChild; i++)
                    genTree(&tree->Inst.cond.scope->children[i]);
            
            break;
        case IT_Ctrl:
            print("exprsn:");
            
                genTree(tree->Inst.cond.exprsn);
            
            print("");
            print("scope:");
            
                for (int i = 0; i < tree->Inst.ctrl.scope->noChild; i++)
                    genTree(&tree->Inst.ctrl.scope->children[i]);
            
            break;
        case IT_Scope:
            break;
        default:
            mccErr("AST gen ERR %s in dump.c", __LINE__);
            break;
    }
    */
    
}

static void genTree (Tree *tree)
{
    genInst(tree);
    for (int i = 0; i < tree->noChild; i++)
        genTree(&tree->children[i]);
}


void genIr (AsmInst **IR, Tree *AST)
{
    genTree(AST);
}

void genS (char *dest, int destSz, AsmInst *IR)
{
    genHeader(dest, destSz);
    //genTree(tree);
}
