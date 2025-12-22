/**
 * AEMgr.cpp
 * @author kisslune 
 */

#include "AELib.h"
#include "Util/Options.h"
#include "Util/WorkList.h"

using namespace SVF;
using namespace SVFUtil;

/// Abstract state updates on an AddrStmt
void AbstractExecution::updateStateOnAddr(const AddrStmt *addr)
{
    const ICFGNode *node = addr->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    as.initObjVar(SVFUtil::cast<ObjVar>(addr->getRHSVar()));
    as[addr->getLHSVarID()] = as[addr->getRHSVarID()];
}


void AbstractExecution::updateStateOnCopy(const CopyStmt *copy)
{
    const ICFGNode *node = copy->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    auto lhs = copy->getLHSVarID();
    auto rhs = copy->getRHSVarID();
    // TODO: your code starts from here
}


void AbstractExecution::updateStateOnStore(const StoreStmt *store)
{
    const ICFGNode *node = store->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    auto lhs = store->getLHSVarID();
    auto rhs = store->getRHSVarID();
    // TODO: your code starts from here
}


void AbstractExecution::updateStateOnLoad(const LoadStmt *load)
{
    const ICFGNode *node = load->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    auto lhs = load->getLHSVarID();
    auto rhs = load->getRHSVarID();
    // TODO: your code starts from here
}


void AbstractExecution::updateStateOnGep(const GepStmt *gep)
{
    AbstractState &as = getAbsStateFromTrace(gep->getICFGNode());
    u32_t rhs = gep->getRHSVarID();
    u32_t lhs = gep->getLHSVarID();
    // TODO: your code starts from here
}


void AbstractExecution::updateStateOnPhi(const PhiStmt *phi)
{
    const ICFGNode *icfgNode = phi->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(icfgNode);
    u32_t res = phi->getResID();
    auto rhs = AbstractValue();
    // TODO: your code starts from here
}


/// You are required to handle predicates (The program is assumed to have signed ints and also interger-overflow-free),
/// including Add, FAdd, Sub, FSub, Mul, FMul, SDiv, FDiv, UDiv, SRem, FRem, URem
void AbstractExecution::updateStateOnBinary(const BinaryOPStmt *binary)
{
    const ICFGNode *node = binary->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    u32_t op0 = binary->getOpVarID(0);
    u32_t op1 = binary->getOpVarID(1);
    u32_t res = binary->getResID();
    if (!as.inVarToValTable(op0)) as[op0] = IntervalValue::top();
    if (!as.inVarToValTable(op1)) as[op1] = IntervalValue::top();
    IntervalValue &lhs = as[op0].getInterval(), &rhs = as[op1].getInterval();
    IntervalValue resVal;
    auto opCode = binary->getOpcode();

    if (opCode == BinaryOPStmt::Add || opCode == BinaryOPStmt::FAdd)
    {
        // TODO: handle add
    }
    else if (opCode == BinaryOPStmt::Sub || opCode == BinaryOPStmt::FSub)
    {
        // TODO: handle subtraction
    }
    else if (opCode == BinaryOPStmt::Mul || opCode == BinaryOPStmt::FMul)
    {
        // TODO: handle multiplication
    }
    else if (opCode == BinaryOPStmt::SDiv || opCode == BinaryOPStmt::FDiv || opCode == BinaryOPStmt::UDiv)
    {
        // TODO: handle division
    }
    else if (opCode == BinaryOPStmt::SRem || opCode == BinaryOPStmt::FRem || opCode == BinaryOPStmt::URem)
    {
        // TODO: handle remainder
    }
    else if (opCode == BinaryOPStmt::Xor)
    {
        resVal = lhs ^ rhs;
    }
    else if (opCode == BinaryOPStmt::And)
    {
        resVal = lhs & rhs;
    }
    else if (opCode == BinaryOPStmt::Or)
    {
        resVal = lhs | rhs;
    }
    else if (opCode == BinaryOPStmt::Shl)
    {
        resVal = lhs << rhs;
    }
    else if (opCode == BinaryOPStmt::AShr || opCode == BinaryOPStmt::LShr)
    {
        resVal = lhs >> rhs;
    }
    as[res] = resVal;
}

/// Handle cycle WTO
void AbstractExecution::handleCycleWTO(const ICFGCycleWTO *cycle)
{
    const ICFGNode *cycle_head = cycle->head()->getICFGNode();
    // Flag to indicate if we are in the increasing phase
    bool increasing = true;
    // Infinite loop until a fixpoint is reached,
    for (u32_t cur_iter = 0;; cur_iter++)
    {
        // TODO: your code start from here
    }
}

/// Abstract state updates on an CallPE
void AbstractExecution::updateStateOnCall(const CallPE *call)
{
    const ICFGNode *node = call->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    NodeID lhs = call->getLHSVarID();
    NodeID rhs = call->getRHSVarID();
    as[lhs] = as[rhs];
}

/// Abstract state updates on an RetPE
void AbstractExecution::updateStateOnRet(const RetPE *retPE)
{
    const ICFGNode *node = retPE->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    NodeID lhs = retPE->getLHSVarID();
    NodeID rhs = retPE->getRHSVarID();
    as[lhs] = as[rhs];
}