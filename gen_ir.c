#include "mcc.h"

static void genIRInst (IrRoutine *ir, Tree *tree);
static void genTree (IrRoutine *ir, Tree *tree);

static Operand opNULL;

/* ===== IR construction helpers ===== */

IrRoutine *crtRoutine (char *name)
{
    IrRoutine *routine = (IrRoutine *)malloc(sizeof(IrRoutine));
    IrInst *inst = (IrInst *)malloc(sizeof(IrInst));

    strcpy(routine->name, name);
    routine->inst = inst;
    routine->inst->end = true;
    routine->end = true;
    routine->frameSize = 0;
    return routine;
}

static IrInst *crtInst (Opcode op, Operand dest, Operand src)
{
    IrInst *inst = (IrInst *)malloc(sizeof(IrInst));
    inst->op = op;
    inst->dest = dest;
    inst->src = src;
    inst->end = true;
    return inst;
}

static Opcode opc (enum OpcodeInstType type, enum OpcodeMemType size)
{
    Opcode opcode;
    opcode.type = type;
    opcode.size = size;
    opcode.cond = CC_EQ;
    return opcode;
}

static Opcode opcc (enum OpcodeInstType type, enum OpcodeMemType size, enum CondCode cond)
{
    Opcode opcode;
    opcode.type = type;
    opcode.size = size;
    opcode.cond = cond;
    return opcode;
}

static Operand ope (enum OperandType type)
{
    Operand operand;
    operand.type = type;
    return operand;
}

static Register regp (enum RegPhyType phy)
{
    Register reg;
    reg.type = RT_phy;
    reg.phy = phy;
    return reg;
}

static Register regr (int offset)
{
    Register reg;
    reg.type = RT_rel;
    reg.rel = offset;
    return reg;
}

static Operand crtOpeReg (Operand operand, Register reg)
{
    operand.reg = reg;
    return operand;
}

static Operand crtOpeStr (Operand operand, char *str)
{
    strncpy(operand.str, str, 127);
    operand.str[127] = '\0';
    return operand;
}

static Operand crtOpeNum (Operand operand, int num)
{
    operand.num = num;
    return operand;
}

static void appendRoutine (IrRoutine *dest, IrRoutine *src)
{
    while (!dest->end)
        dest = dest->next;
    dest->end = false;
    src->end = true;
    dest->next = src;
}

static void appendInst (IrRoutine *routine, IrInst *src)
{
    IrInst *dest;
    dest = routine->inst;
    while (!dest->end)
        dest = dest->next;
    dest->end = false;
    src->end = true;
    dest->next = src;
}

/* ===== Emit helpers ===== */

static void emitComment (IrRoutine *ir, char *msg)
{
    appendInst(ir, crtInst(opc(OIT_comment, OMT_long),
        crtOpeStr(ope(OT_comment), msg), opNULL));
}

static void emitMov (IrRoutine *ir, enum RegPhyType dst, enum RegPhyType src)
{
    appendInst(ir, crtInst(opc(OIT_mov, OMT_long),
        crtOpeReg(ope(OT_reg), regp(dst)),
        crtOpeReg(ope(OT_reg), regp(src))));
}

static void emitMovImm (IrRoutine *ir, enum RegPhyType dst, int imm)
{
    appendInst(ir, crtInst(opc(OIT_mov, OMT_long),
        crtOpeReg(ope(OT_reg), regp(dst)),
        crtOpeNum(ope(OT_num_lit), imm)));
}

static void emitPush (IrRoutine *ir, enum RegPhyType reg)
{
    appendInst(ir, crtInst(opc(OIT_push, OMT_long),
        crtOpeReg(ope(OT_reg), regp(reg)), opNULL));
}

static void emitPop (IrRoutine *ir, enum RegPhyType reg)
{
    appendInst(ir, crtInst(opc(OIT_pop, OMT_long),
        crtOpeReg(ope(OT_reg), regp(reg)), opNULL));
}

static void emitLoad (IrRoutine *ir, enum RegPhyType dst, int stackOff)
{
    appendInst(ir, crtInst(opc(OIT_ldr, OMT_long),
        crtOpeReg(ope(OT_reg), regp(dst)),
        crtOpeReg(ope(OT_reg), regr(stackOff))));
}

static void emitLoadReg (IrRoutine *ir, enum RegPhyType dst, enum RegPhyType addrReg)
{
    appendInst(ir, crtInst(opc(OIT_ldr, OMT_long),
        crtOpeReg(ope(OT_reg), regp(dst)),
        crtOpeReg(ope(OT_reg), regp(addrReg))));
}

static void emitStore (IrRoutine *ir, enum RegPhyType src, int stackOff)
{
    appendInst(ir, crtInst(opc(OIT_str, OMT_long),
        crtOpeReg(ope(OT_reg), regp(src)),
        crtOpeReg(ope(OT_reg), regr(stackOff))));
}

static void emitStoreReg (IrRoutine *ir, enum RegPhyType src, enum RegPhyType addrReg)
{
    appendInst(ir, crtInst(opc(OIT_str, OMT_long),
        crtOpeReg(ope(OT_reg), regp(src)),
        crtOpeReg(ope(OT_reg), regp(addrReg))));
}

static void emitArith (IrRoutine *ir, enum OpcodeInstType op)
{
    appendInst(ir, crtInst(opc(op, OMT_long),
        crtOpeReg(ope(OT_reg), regp(RAT_x9)),
        crtOpeReg(ope(OT_reg), regp(RAT_x10))));
}

static void emitCmpReg (IrRoutine *ir, enum RegPhyType left, enum RegPhyType right)
{
    appendInst(ir, crtInst(opc(OIT_cmp, OMT_long),
        crtOpeReg(ope(OT_reg), regp(left)),
        crtOpeReg(ope(OT_reg), regp(right))));
}

static void emitCmpImm (IrRoutine *ir, enum RegPhyType reg, int imm)
{
    appendInst(ir, crtInst(opc(OIT_cmp, OMT_long),
        crtOpeReg(ope(OT_reg), regp(reg)),
        crtOpeNum(ope(OT_num_lit), imm)));
}

static void emitCset (IrRoutine *ir, enum RegPhyType dst, enum CondCode cond)
{
    appendInst(ir, crtInst(opcc(OIT_cset, OMT_long, cond),
        crtOpeReg(ope(OT_reg), regp(dst)), opNULL));
}

static void emitBranch (IrRoutine *ir, enum OpcodeInstType brType, char *label)
{
    appendInst(ir, crtInst(opc(brType, OMT_long),
        crtOpeStr(ope(OT_str_routine), label), opNULL));
}

static void emitLabel (IrRoutine *ir, char *label)
{
    appendInst(ir, crtInst(opc(OIT_label, OMT_long),
        crtOpeStr(ope(OT_str_routine), label), opNULL));
}

static void emitCall (IrRoutine *ir, char *funcName)
{
    appendInst(ir, crtInst(opc(OIT_call, OMT_long),
        crtOpeStr(ope(OT_str_routine), funcName), opNULL));
}

static void emitRet (IrRoutine *ir)
{
    appendInst(ir, crtInst(opc(OIT_ret, OMT_long), opNULL, opNULL));
}

/* ===== Variable tracking ===== */

typedef struct
{
    char name[128];
    int memLoc;
    bool isArray;
    int arraySize;
    enum LitType type;
    char typeName[128];
} IrVar;

static IrVar vars[128];
static int varSz = 0;
static int scopeLocation;

static void resetFuncState ()
{
    varSz = 0;
    scopeLocation = 16; // start after saved FP/LR (16 bytes)
}

static void crtVar (char *varName, bool isArray, int arraySize, enum LitType type)
{
    int allocSize = isArray ? arraySize * 8 : 8;

    strcpy(vars[varSz].name, varName);
    vars[varSz].memLoc = scopeLocation;
    vars[varSz].isArray = isArray;
    vars[varSz].arraySize = arraySize;
    vars[varSz].type = type;
    vars[varSz].typeName[0] = '\0';
    varSz++;
    scopeLocation += allocSize;
}

static int getVarMemLoc (char *varName)
{
    for (int i = 0; i < varSz; i++)
    {
        if (strcmp(vars[i].name, varName) == 0)
            return vars[i].memLoc;
    }
    return 0;
}

static bool isVarArray (char *varName)
{
    for (int i = 0; i < varSz; i++)
    {
        if (strcmp(vars[i].name, varName) == 0)
            return vars[i].isArray;
    }
    return false;
}

static char *getVarTypeName (char *varName)
{
    for (int i = 0; i < varSz; i++)
    {
        if (strcmp(vars[i].name, varName) == 0)
            return vars[i].typeName;
    }
    return "";
}

/* ===== Label counter ===== */

static int labelCounter = 0;

/* ===== Arg register helper ===== */

static enum RegPhyType argReg (int i)
{
    switch (i)
    {
        case 0: return RAT_x0;
        case 1: return RAT_x1;
        case 2: return RAT_x2;
        case 3: return RAT_x3;
        case 4: return RAT_x4;
        case 5: return RAT_x5;
        case 6: return RAT_x6;
        case 7: return RAT_x7;
        default: return RAT_x0;
    }
}

/* ===== String literal table (Phase 7) ===== */

typedef struct
{
    char label[32];
    char value[256];
} StringEntry;

static StringEntry stringTable[128];
static int stringTableSz = 0;

int getStringLitCount () { return stringTableSz; }
char *getStringLitLabel (int i) { return stringTable[i].label; }
char *getStringLitValue (int i) { return stringTable[i].value; }

/* ===== Struct/Union layout table (Phase 8) ===== */

typedef struct
{
    char fieldName[128];
    int offset;
    int size;
} FieldLayout;

typedef struct
{
    char typeName[128];
    FieldLayout fields[32];
    int fieldCount;
    int totalSize;
    bool isUnion;
} TypeLayout;

static TypeLayout typeLayouts[64];
static int typeLayoutCount = 0;

static int getFieldOffset (char *typeName, char *fieldName)
{
    for (int i = 0; i < typeLayoutCount; i++)
    {
        if (strcmp(typeLayouts[i].typeName, typeName) == 0)
        {
            for (int j = 0; j < typeLayouts[i].fieldCount; j++)
            {
                if (strcmp(typeLayouts[i].fields[j].fieldName, fieldName) == 0)
                    return typeLayouts[i].fields[j].offset;
            }
        }
    }
    return 0;
}

static void registerStructLayout (Tree *tree, bool isUnion)
{
    char *name = isUnion ? tree->ast.unin.uninName : tree->ast.strct.strctName;
    Tree *decls = isUnion ? tree->ast.unin.decls : tree->ast.strct.decls;

    TypeLayout *layout = &typeLayouts[typeLayoutCount];
    strcpy(layout->typeName, name);
    layout->isUnion = isUnion;
    layout->fieldCount = 0;
    layout->totalSize = 0;

    int offset = 0;
    for (int i = 0; i < decls->childrenSz; i++)
    {
        Tree *field = &decls->children[i];
        if (field->ast.type == IT_Var)
        {
            FieldLayout *fl = &layout->fields[layout->fieldCount];
            strcpy(fl->fieldName, field->ast.var.varName);
            fl->size = 8;
            fl->offset = isUnion ? 0 : offset;
            layout->fieldCount++;
            if (!isUnion)
                offset += 8;
        }
    }
    layout->totalSize = isUnion ? 8 : offset;
    if (layout->totalSize == 0)
        layout->totalSize = 8;

    typeLayoutCount++;
}

/* ===== Allocate struct variable with type info ===== */

static void crtVarTyped (char *varName, bool isArray, int arraySize, enum LitType type)
{
    if (type >= 3 && (type - 3) < typeLayoutCount)
    {
        TypeLayout *tl = &typeLayouts[type - 3];
        int allocSize = isArray ? arraySize * tl->totalSize : tl->totalSize;

        strcpy(vars[varSz].name, varName);
        vars[varSz].memLoc = scopeLocation;
        vars[varSz].isArray = isArray;
        vars[varSz].arraySize = arraySize;
        vars[varSz].type = type;
        strcpy(vars[varSz].typeName, tl->typeName);
        varSz++;
        scopeLocation += allocSize;
    }
    else
    {
        crtVar(varName, isArray, arraySize, type);
    }
}

/* ===== Address computation helper ===== */

static void emitAddrOf (IrRoutine *ir, int stackOff)
{
    emitMov(ir, RAT_x9, RAT_fp);
    if (stackOff != 0)
    {
        appendInst(ir, crtInst(opc(OIT_add, OMT_long),
            crtOpeReg(ope(OT_reg), regp(RAT_x9)),
            crtOpeNum(ope(OT_num_lit), stackOff)));
    }
}

/* ===== Main IR generation ===== */

static void genIRInst (IrRoutine *ir, Tree *tree)
{
    IrRoutine *irChild;
    char label[128];
    char label2[128];
    int lbl;
    int memLoc;
    Tree *lhs;

    switch (tree->ast.type)
    {
        case IT_Var:
            mccLog("ir: var %s", tree->ast.var.varName);
            crtVarTyped(tree->ast.var.varName,
                        tree->ast.var.isArray,
                        tree->ast.var.isArray && tree->ast.var.arrayLength
                            ? tree->ast.var.arrayLength->ast.lit.val.tInt : 0,
                        tree->ast.var.varType);
            break;

        case IT_Func:
            if (tree->ast.func.scope->childrenSz == 0)
                break;

            mccLog("ir: func %s", tree->ast.func.funcName);
            irChild = crtRoutine(tree->ast.func.funcName);
            appendRoutine(ir, irChild);
            resetFuncState();

            emitComment(irChild, "function body");

            /* store incoming parameters to stack slots */
            for (int i = 0; i < tree->ast.func.parameters->childrenSz; i++)
            {
                Tree *param = &tree->ast.func.parameters->children[i];
                crtVar(param->ast.var.varName, false, 0, param->ast.var.varType);
                memLoc = getVarMemLoc(param->ast.var.varName);
                emitStore(irChild, argReg(i), memLoc);
            }

            /* process function body */
            for (int i = 0; i < tree->ast.func.scope->childrenSz; i++)
                genTree(irChild, &tree->ast.func.scope->children[i]);

            /* compute frame size: total = scopeLocation, round up to 16 */
            {
                int totalFrame = (scopeLocation + 15) & ~15;
                if (totalFrame < 16)
                    totalFrame = 16;
                irChild->frameSize = totalFrame;
            }
            break;

        case IT_Lit:
            mccLog("ir: lit");
            if (tree->ast.lit.type == LT_INT)
            {
                emitMovImm(ir, RAT_x9, tree->ast.lit.val.tInt);
            }
            else if (tree->ast.lit.type == LT_CHAR)
            {
                if (tree->ast.lit.val.tStr[0] == '"')
                {
                    /* string literal -> store in string table, load address */
                    sprintf(label, ".Lstr_%d", stringTableSz);
                    strcpy(stringTable[stringTableSz].label, label);
                    strncpy(stringTable[stringTableSz].value,
                            tree->ast.lit.val.tStr + 1,
                            sizeof(stringTable[0].value) - 1);
                    {
                        int len = strlen(stringTable[stringTableSz].value);
                        if (len > 0 && stringTable[stringTableSz].value[len-1] == '"')
                            stringTable[stringTableSz].value[len-1] = '\0';
                    }
                    stringTableSz++;
                    appendInst(ir, crtInst(opc(OIT_adr, OMT_long),
                        crtOpeReg(ope(OT_reg), regp(RAT_x9)),
                        crtOpeStr(ope(OT_str_routine), label)));
                }
                else
                {
                    emitMovImm(ir, RAT_x9, (int)tree->ast.lit.val.tChar);
                }
            }
            break;

        case IT_Id:
            mccLog("ir: id %s", tree->ast.id.varName);
            memLoc = getVarMemLoc(tree->ast.id.varName);

            if (tree->ast.id.nested && tree->ast.id.nested->childrenSz > 0)
            {
                /* struct member access: s.field */
                char *typeName = getVarTypeName(tree->ast.id.varName);
                char *fieldName = tree->ast.id.nested->children[0].ast.id.varName;
                int fieldOff = getFieldOffset(typeName, fieldName);
                emitLoad(ir, RAT_x9, memLoc + fieldOff);
            }
            else if (isVarArray(tree->ast.id.varName))
            {
                emitAddrOf(ir, memLoc);
            }
            else
            {
                emitLoad(ir, RAT_x9, memLoc);
            }
            break;

        case IT_Assign:
            mccLog("ir: assign");
            lhs = tree->ast.assign.varName;

            /* evaluate right-hand side expression -> result in x9 */
            genTree(ir, tree->ast.assign.exprsn);

            if (lhs->ast.type == IT_Deref)
            {
                /* pointer store: *ptr = value */
                emitPush(ir, RAT_x9);
                genTree(ir, lhs->ast.deref.exprsn);
                emitMov(ir, RAT_x10, RAT_x9);
                emitPop(ir, RAT_x9);
                emitStoreReg(ir, RAT_x9, RAT_x10);
            }
            else if (lhs->ast.type == IT_Id)
            {
                memLoc = getVarMemLoc(lhs->ast.id.varName);

                if (lhs->ast.id.nested && lhs->ast.id.nested->childrenSz > 0)
                {
                    /* struct member store: s.field = value */
                    char *typeName = getVarTypeName(lhs->ast.id.varName);
                    char *fieldName = lhs->ast.id.nested->children[0].ast.id.varName;
                    int fieldOff = getFieldOffset(typeName, fieldName);
                    emitStore(ir, RAT_x9, memLoc + fieldOff);
                }
                else
                {
                    emitStore(ir, RAT_x9, memLoc);
                }
            }
            break;

        case IT_Binary:
            mccLog("ir: binary op %s", tree->ast.binary.op.id);

            /* evaluate left side -> x9 */
            genTree(ir, tree->ast.binary.left);
            emitPush(ir, RAT_x9);

            /* evaluate right side -> x9, then move to x10 */
            genTree(ir, tree->ast.binary.right);
            emitMov(ir, RAT_x10, RAT_x9);

            /* pop left back to x9 */
            emitPop(ir, RAT_x9);

            /* apply operator: result in x9 */
            if (strcmp(tree->ast.binary.op.id, "+") == 0)
                emitArith(ir, OIT_add);
            else if (strcmp(tree->ast.binary.op.id, "-") == 0)
                emitArith(ir, OIT_sub);
            else if (strcmp(tree->ast.binary.op.id, "*") == 0)
                emitArith(ir, OIT_mul);
            else if (strcmp(tree->ast.binary.op.id, "/") == 0)
                emitArith(ir, OIT_sdiv);
            else if (strcmp(tree->ast.binary.op.id, "==") == 0)
            {
                emitCmpReg(ir, RAT_x9, RAT_x10);
                emitCset(ir, RAT_x9, CC_EQ);
            }
            else if (strcmp(tree->ast.binary.op.id, "!=") == 0)
            {
                emitCmpReg(ir, RAT_x9, RAT_x10);
                emitCset(ir, RAT_x9, CC_NE);
            }
            else if (strcmp(tree->ast.binary.op.id, ">") == 0)
            {
                emitCmpReg(ir, RAT_x9, RAT_x10);
                emitCset(ir, RAT_x9, CC_GT);
            }
            else if (strcmp(tree->ast.binary.op.id, "&&") == 0)
            {
                emitCmpImm(ir, RAT_x9, 0);
                emitCset(ir, RAT_x9, CC_NE);
                emitCmpImm(ir, RAT_x10, 0);
                emitCset(ir, RAT_x10, CC_NE);
                emitArith(ir, OIT_and);
            }
            else if (strcmp(tree->ast.binary.op.id, "||") == 0)
            {
                emitCmpImm(ir, RAT_x9, 0);
                emitCset(ir, RAT_x9, CC_NE);
                emitCmpImm(ir, RAT_x10, 0);
                emitCset(ir, RAT_x10, CC_NE);
                emitArith(ir, OIT_orr);
            }
            else if (strcmp(tree->ast.binary.op.id, "&") == 0)
            {
                emitArith(ir, OIT_and);
            }
            else if (strcmp(tree->ast.binary.op.id, ">>") == 0)
            {
                emitArith(ir, OIT_lsr);
            }
            break;

        case IT_Ret:
            mccLog("ir: return");
            genTree(ir, tree->ast.ret.exprsn);
            emitMov(ir, RAT_x0, RAT_x9);
            emitRet(ir);
            break;

        case IT_Call:
            mccLog("ir: call %s", tree->ast.call.funcName);

            /* evaluate each argument and push onto stack */
            for (int i = 0; i < tree->ast.call.args->childrenSz; i++)
            {
                genTree(ir, &tree->ast.call.args->children[i]);
                emitPush(ir, RAT_x9);
            }

            /* pop args into registers in reverse order */
            for (int i = tree->ast.call.args->childrenSz - 1; i >= 0; i--)
                emitPop(ir, argReg(i));

            emitCall(ir, tree->ast.call.funcName);
            emitMov(ir, RAT_x9, RAT_x0);
            break;

        case IT_Cond:
            mccLog("ir: if");
            lbl = labelCounter++;
            sprintf(label, ".L_endif_%d", lbl);

            genTree(ir, tree->ast.cond.exprsn);
            emitCmpImm(ir, RAT_x9, 0);
            emitBranch(ir, OIT_beq, label);

            for (int i = 0; i < tree->ast.cond.scope->childrenSz; i++)
                genTree(ir, &tree->ast.cond.scope->children[i]);

            emitLabel(ir, label);
            break;

        case IT_Ctrl:
            mccLog("ir: while");
            lbl = labelCounter++;
            sprintf(label, ".L_while_top_%d", lbl);
            sprintf(label2, ".L_while_end_%d", lbl);

            emitLabel(ir, label);

            genTree(ir, tree->ast.ctrl.exprsn);
            emitCmpImm(ir, RAT_x9, 0);
            emitBranch(ir, OIT_beq, label2);

            for (int i = 0; i < tree->ast.ctrl.scope->childrenSz; i++)
                genTree(ir, &tree->ast.ctrl.scope->children[i]);

            emitBranch(ir, OIT_jmp, label);
            emitLabel(ir, label2);
            break;

        case IT_Deref:
            mccLog("ir: deref");
            genTree(ir, tree->ast.deref.exprsn);
            emitLoadReg(ir, RAT_x9, RAT_x9);
            break;

        case IT_Ref:
            mccLog("ir: ref");
            if (tree->ast.ref.ptr->ast.type == IT_Id)
            {
                memLoc = getVarMemLoc(tree->ast.ref.ptr->ast.id.varName);
                emitAddrOf(ir, memLoc);
            }
            break;

        case IT_Strct:
            mccLog("ir: struct %s", tree->ast.strct.strctName);
            registerStructLayout(tree, false);
            break;

        case IT_Unin:
            mccLog("ir: union %s", tree->ast.unin.uninName);
            registerStructLayout(tree, true);
            break;

        case IT_Scope:
            break;

        default:
            break;
    }
}

static void genTree (IrRoutine *ir, Tree *tree)
{
    genIRInst(ir, tree);
}

void genIr (IrRoutine *ir, Tree *ast)
{
    for (int i = 0; i < ast->childrenSz; i++)
        genTree(ir, &ast->children[i]);
}
