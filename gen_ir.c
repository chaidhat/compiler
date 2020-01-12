#include "mcc.h"
static char datBuf[DB_SIZE];

static DagInst crtInst (enum DagType type);
static void bindInst (Tree *parent, DagInst inst);

static void genInstOp (Tree *parent, Operand op);
static void genInstStr (Tree *parent, enum DagType type, char str[128]);
static void genInstNum (Tree *parent, int num);

static void genIRInst (Tree *treeOut, Tree *treeIn);
static void genTree (Tree *treeOut, Tree *treeIn);

static DagInst crtInst (enum DagType type)
{
    DagInst inst;
    inst.type = type;

    return inst;
}

static void bindInst (Tree *parent, DagInst inst)
{
    Tree tree;
    tree.childrenSz = 0;
    tree.dag = inst;

    appendChild(parent, tree);
}

static void genInstOp (Tree *parent, Operand op)
{
    DagInst inst;
    inst = crtInst(DT_operand);
    inst.operand = op;
    bindInst(parent, inst);
}

static void genInstStr (Tree *parent, enum DagType type, char str[128])
{
    DagInst inst;
    inst = crtInst(type);
    strcpy(inst.str, str);
    bindInst(parent, inst);
}

static void genInstNum (Tree *parent, int num)
{
    DagInst inst;
    inst = crtInst(DT_num);
    inst.num = num;
    bindInst(parent, inst);
}
static void genIRInst (Tree *treeOut, Tree *treeIn)
{
    switch (treeIn->ast.type)
    {
        case IT_Var:
            mccLog("var");
            break;
        case IT_Func:
            genInstStr(treeOut, DT_func, treeIn->ast.func.funcName);
            mccLog("%s %d", treeOut->children[treeOut->childrenSz - 1].dag.str, treeOut->childrenSz);

            // scope
            for (int i = 0; i < treeIn->ast.func.scope->childrenSz; i++)
                genTree(treeIn, &treeIn->ast.func.scope->children[i]);

            break;
        case IT_Lit:
            genInstNum(treeOut, treeIn->ast.lit.val.tInt);
            mccLog("lit %d", treeOut->children[treeOut->childrenSz - 1].dag.num);
            /*
            print("type: %s", LTtostr(tree->Inst.lit.type));
            if (tree->Inst.lit.type == LT_INT)
                print("val: %d", tree->Inst.lit.val.tInt);
            else
                mccErr("val: CHAR NOT PROG"); // TODO: char
            */
            break;
        case IT_Id:
            /*
            print("isPtr: %d", tree->Inst.id.isPtr);
            print("varName: %s", tree->Inst.id.varName);
            if (tree->Inst.id.nested->noChild > 0)
            {
                print("");
                print("nested:");
                 
                    for (int i = 0; i < tree->Inst.id.nested->noChild; i++)
                        genTree(&tree->Inst.id.nested->children[i]);
                
            }
            */
            break;
        case IT_Assign:
            mccLog("assign");
             
                // varname
                genTree(treeIn, treeIn->ast.assign.varName);
            
                // exprsn
                genTree(treeIn, treeIn->ast.assign.exprsn);
            
            break;
        default:
            break;
    }
     
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

static void genTree (Tree *treeOut, Tree *treeIn)
{
    genIRInst(treeOut, treeIn);
    for (int i = 0; i < treeIn->childrenSz; i++)
        genTree(treeOut, &treeIn->children[i]);
}


void genIr (Tree *IrDag, Tree *AST)
{
    genTree(IrDag, AST);
}
