#include "mcc.h"
static char datBuf[DB_SIZE];

static void genIRInst (IrRoutine *ir, Tree *treeIn);
static void genTree (IrRoutine *ir, Tree *treeIn);

static Operand opNULL;

IrRoutine *createRoutine (char *name)
{
    IrRoutine *routine = (IrRoutine *)malloc(sizeof(IrRoutine));
    IrInst *inst = (IrInst *)malloc(sizeof(IrInst));

    strcpy(routine->name, name);
    routine->inst = inst; // first inst is an inaccessible stub
    routine->end = true;
    return routine;
}

static IrInst *createInst (Opcode op, Operand dest, Operand src)
{
    IrInst *inst = (IrInst *)malloc(sizeof(IrInst));

    inst->op = op;
    inst->dest = dest;
    inst->src = src;
    return inst;
}
static Opcode opc (enum OpcodeInstType type, enum OpcodeMemType size)
{
    Opcode opcode;

    opcode.type = type;
    opcode.size = size;

    return opcode;
}

static Register reg (int regnum) // for infinite registers
{
    Register reg;
    reg.regnum = regnum;
}

static Operand ope (enum OperandType type)
{
    Operand operand;
    operand.type = type;
    return operand;
}

static void appendRoutine (IrRoutine *dest, IrRoutine *src)
{
    dest->end = false;
    src->end = true; // assumes src is the last element
    dest->next = src;
}

static void appendInst (IrRoutine *routine, IrInst *src)
{
    IrInst *dest;
    dest = routine->inst; // first inst is stub
    while (!dest->end)
        dest = dest->next; // first inst is stub
    dest->end = false;
    src->end = true; // assumes src is the last element
    dest->next = src;
}

static void genIRInst (IrRoutine *ir, Tree *treeIn)
{
    Tree *thisTree;
    Operand opL;
    Operand opR;
    switch (treeIn->ast.type)
    {
        case IT_Var:
            mccLog("var");
            opL = ope(OT_num_lit);
            opL.num = 64;
            appendInst(ir, createInst(opc(OIT_push, OMT_long), opL, opNULL));
            //genVar(treeOut, treeIn->ast.var.varName);
            break;
        case IT_Func:
            //genFunc(treeOut, treeIn->ast.func.funcName);
            mccLog("func");

            //thisTree = &treeOut->children[treeOut->childrenSz - 1];

            // scope
            //bindScope(thisTree);
            for (int i = 0; i < treeIn->ast.func.scope->childrenSz; i++)
                genTree(ir, &treeIn->ast.func.scope->children[i]);
            //logTree(&thisTree->children[0]);

            break;
        case IT_Lit:
            opL = ope(OT_str_lit);
            opR = ope(OT_str_lit);
            strcpy(opL.str, "alternating current");
            strcpy(opR.str, "b");
            appendInst(ir, createInst(opc(OIT_push, OMT_long), opL, opR));
            //genInstNum(treeOut, treeIn->ast.lit.val.tInt);
            //mccLog("lit %d", treeOut->children[treeOut->childrenSz - 1].dag.num);
            /*
            print("type: %s", LTtostr(tree->Inst.lit.type));
            if (tree->Inst.lit.type == LT_INT)
                print("val: %d", tree->Inst.lit.val.tInt);
            else
                mccErr("val: CHAR NOT PROG"); // TODO: char
            */
            break;
        case IT_Id:
            //genInstStr(treeOut, treeIn->ast.id.varName);
            //mccLog("id %s", treeOut->children[treeOut->childrenSz - 1].dag.str);
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
             
            //thisTree = &treeOut->children[treeOut->childrenSz - 1];
            // varname
            //bindScope(thisTree);
            genTree(ir, treeIn->ast.assign.varName);
            
            // exprsn
            //bindScope(thisTree);
            genTree(ir, treeIn->ast.assign.exprsn);
            
            break;
        default:
            break;
    }
     
}

static void genTree (IrRoutine *ir, Tree *treeIn)
{
    genIRInst(ir, treeIn);
    for (int i = 0; i < treeIn->childrenSz; i++)
        genTree(ir, &treeIn->children[i]);
}

void genIr (IrRoutine *ir, Tree *ast)
{
    genTree(ir, ast);
}
