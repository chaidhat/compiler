#include "mcc.h"
static char datBuf[DB_SIZE];

static void genIRInst (IrRoutine *ir, Tree *treeIn);
static void genTree (IrRoutine *ir, Tree *treeIn);

static Operand opNULL;

IrRoutine *crtRoutine (char *name)
{
    IrRoutine *routine = (IrRoutine *)malloc(sizeof(IrRoutine));
    IrInst *inst = (IrInst *)malloc(sizeof(IrInst));

    strcpy(routine->name, name);
    routine->inst = inst; // first inst is an inaccessible stub
    routine->inst->end = true;
    routine->end = true;
    return routine;
}

static IrInst *crtInst (Opcode op, Operand dest, Operand src)
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

static Operand ope (enum OperandType type)
{
    Operand operand;
    operand.type = type;
    return operand;
}

static Register regn (char stat[128]) // for infinite registers
{
    Register reg;
    reg.type = RT_stat;
    strcpy(reg.stat, stat);
    return reg;
}

static Register rego (int offset) // for infinite registers
{
    Register reg;
    reg.type = RT_abs;
    reg.abs = offset;
    return reg;
}

static Register regp (enum RegPhyType phy) // for infinite registers
{
    Register reg;
    reg.type = RT_phy;
    reg.phy = phy;
    return reg;
}

static Operand crtOpeReg(Operand operand, Register reg)
{
    operand.reg = reg;
    return operand;
}

static Operand crtOpeStr(Operand operand, char str[128])
{
    mccstr(operand.str, 128, str);
    return operand;
}

static Operand crtOpeNum(Operand operand, int num)
{
    operand.num = num;
    return operand;
}

static void appendRoutine (IrRoutine *dest, IrRoutine *src)
{
    while (!dest->end)
        dest = dest->next; // first inst is stub
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

static void genIRInst (IrRoutine *ir, Tree *tree)
{
    Tree *thisTree;
    IrRoutine *irChild;
    Operand opL;
    Operand opR;
    switch (tree->ast.type)
    {
        case IT_Var:
            mccLog("var");
            appendInst(ir, 
                    crtInst(opc(OIT_push, OMT_long), 
                    crtOpeNum(ope(OT_num_lit), 0),
                    opNULL));
            //genVar(treeOut, treeIn->ast.var.varName);
            break;
        case IT_Func:
            mccLog("func %s",tree->ast.func.funcName);

            irChild = crtRoutine(tree->ast.func.funcName);
            appendRoutine(ir, irChild);

            // subroutine prologue
            appendInst(irChild, 
                crtInst(opc(OIT_comment, OMT_long),
                crtOpeStr(ope(OT_comment), "subroutine prologue"),
                opNULL
            ));
            appendInst(irChild, 
                crtInst(opc(OIT_push, OMT_long),
                crtOpeReg(ope(OT_reg), regp(RAT_ebp)),
                opNULL
            ));
            appendInst(irChild, 
                crtInst(opc(OIT_mov, OMT_long),
                crtOpeReg(ope(OT_reg), regp(RAT_esp)),
                crtOpeReg(ope(OT_reg), regp(RAT_ebp))
            ));

            // main
            appendInst(irChild, 
                crtInst(opc(OIT_comment, OMT_long),
                crtOpeStr(ope(OT_comment), "subroutine main"),
                opNULL
            ));

            for (int i = 0; i < tree->ast.func.scope->childrenSz; i++)
            {
                genTree(irChild, &tree->ast.func.scope->children[i]);
            }
            
            // subroutine epilogue
            appendInst(irChild, 
                crtInst(opc(OIT_comment, OMT_long),
                crtOpeStr(ope(OT_comment), "subroutine epilogue"),
                opNULL
            ));
            appendInst(irChild, 
                crtInst(opc(OIT_mov, OMT_long),
                crtOpeReg(ope(OT_reg), regp(RAT_ebp)),
                crtOpeReg(ope(OT_reg), regp(RAT_esp))
            ));
            appendInst(irChild, 
                crtInst(opc(OIT_pop, OMT_long),
                crtOpeReg(ope(OT_reg), regp(RAT_ebp)),
                opNULL
            ));

            // subroutine return
            appendInst(irChild, 
                crtInst(opc(OIT_comment, OMT_long),
                crtOpeStr(ope(OT_comment), "subroutine return"),
                opNULL
            ));
            appendInst(irChild, 
                crtInst(opc(OIT_pop, OMT_long),
                crtOpeReg(ope(OT_reg), regp(RAT_a)),
                opNULL
            ));
            appendInst(irChild, 
                crtInst(opc(OIT_jmp, OMT_long),
                crtOpeReg(ope(OT_reg), regp(RAT_a)),
                opNULL
            ));

            break;
        case IT_Lit:
            mccLog("lit");
            if (tree->ast.lit.type == LT_INT)
            {
            mccLog("lit");
                appendInst(ir, 
                    crtInst(opc(OIT_mov, OMT_long),
                    crtOpeNum(ope(OT_num_lit), tree->ast.lit.val.tInt),
                    crtOpeReg(ope(OT_reg), regp(RAT_a))
                ));
            }
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
            genTree(ir, tree->ast.assign.varName);
            
            // exprsn
            //bindScope(thisTree);
            genTree(ir, tree->ast.assign.exprsn);
            
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
    for (int i = 0; i < ast->childrenSz; i++)
        genTree(ir, &ast->children[i]);
}
