#include "mcc.h"
static char datBuf[DB_SIZE];

static DagInst crtInst (enum DagType type);
static void bindInst (Tree *parent, DagInst inst);

static void genVar (Tree *parent, char str[128]);
static void genFunc (Tree *parent, char str[128]);
static void genInstOp (Tree *parent, Operand op);
static void genInstStr (Tree *parent, char str[128]);
static void genInstNum (Tree *parent, int num);

static void genIRInst (Tree *treeOut, Tree *treeIn);
static void genTree (Tree *treeOut, Tree *treeIn);

static DagInst crtInst (enum DagType type)
{
    DagInst inst;
    inst.type = type;

    return inst;
}

static void bindScope (Tree *parent)
{
    Tree scope;
    scope.childrenSz = 0;
    strcpy(scope.id, "scope");
    appendChild(parent, scope);
}

static void bindInst (Tree *parent, DagInst inst)
{
    Tree tree;
    tree.childrenSz = 0;
    tree.dag = inst;
    strcpy(tree.id, "inst");

    appendChild(parent, tree);
}

static void genVar (Tree *parent, char str[128])
{
    DagInst inst = crtInst(DT_var);
    strcpy(inst.str, str);
    bindInst(parent, inst);
}

static void genFunc (Tree *parent, char str[128])
{
    DagInst inst = crtInst(DT_func);
    strcpy(inst.str, str);
    bindInst(parent, inst);
}

static void genInstOp (Tree *parent, Operand op)
{
    DagInst inst = crtInst(DT_operand);
    inst.operand = op;
    bindInst(parent, inst);
}

static void genInstStr (Tree *parent, char str[128])
{
    DagInst inst = crtInst(DT_str);
    strcpy(inst.str, str);
    bindInst(parent, inst);
}

static void genInstNum (Tree *parent, int num)
{
    DagInst inst = crtInst(DT_num);
    inst.num = num;
    bindInst(parent, inst);
}
static void genIRInst (Tree *treeOut, Tree *treeIn)
{
    Tree *thisTree;
    switch (treeIn->ast.type)
    {
        case IT_Var:
            genVar(treeOut, treeIn->ast.var.varName);
            mccLog("var %s", treeOut->children[treeOut->childrenSz - 1].dag.str);
            break;
        case IT_Func:
            genFunc(treeOut, treeIn->ast.func.funcName);
            mccLog("func %s %d", treeOut->children[treeOut->childrenSz - 1].dag.str, treeOut->childrenSz);

            thisTree = &treeOut->children[treeOut->childrenSz - 1];


            // scope
            bindScope(thisTree);
            for (int i = 0; i < treeIn->ast.func.scope->childrenSz; i++)
                genTree(&thisTree->children[0], &treeIn->ast.func.scope->children[i]);
            logTree(&thisTree->children[0]);

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
            genInstStr(treeOut, treeIn->ast.id.varName);
            mccLog("id %s", treeOut->children[treeOut->childrenSz - 1].dag.str);
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
             
            thisTree = &treeOut->children[treeOut->childrenSz - 1];
            // varname
            bindScope(thisTree);
            genTree(&thisTree->children[0], treeIn->ast.assign.varName);
            
            // exprsn
            bindScope(thisTree);
            genTree(&thisTree->children[1], treeIn->ast.assign.exprsn);
            
            break;
        default:
            break;
    }
     
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
