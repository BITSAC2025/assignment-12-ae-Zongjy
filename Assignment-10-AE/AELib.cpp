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
    // Copy the value from rhs to lhs
    as[lhs] = as[rhs];
}


void AbstractExecution::updateStateOnStore(const StoreStmt *store)
{
    const ICFGNode *node = store->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    auto lhs = store->getLHSVarID();
    auto rhs = store->getRHSVarID();
    // Store: *lhs = rhs, store the value of rhs to the memory location pointed by lhs
    if (as.inVarToAddrsTable(lhs))
    {
        for (const auto &addr : as[lhs].getAddrs())
        {
            as.store(addr, as[rhs]);
        }
    }
}


void AbstractExecution::updateStateOnLoad(const LoadStmt *load)
{
    const ICFGNode *node = load->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(node);
    auto lhs = load->getLHSVarID();
    auto rhs = load->getRHSVarID();
    // Load: lhs = *rhs, load the value from the memory location pointed by rhs
    if (as.inVarToAddrsTable(rhs))
    {
        AbstractValue res;
        for (const auto &addr : as[rhs].getAddrs())
        {
            res.join_with(as.load(addr));
        }
        as[lhs] = res;
    }
}


void AbstractExecution::updateStateOnGep(const GepStmt *gep)
{
    AbstractState &as = getAbsStateFromTrace(gep->getICFGNode());
    u32_t rhs = gep->getRHSVarID();
    u32_t lhs = gep->getLHSVarID();
    // GEP (Get Element Pointer): calculate the address of a field/element
    // Get the byte offset from the GEP statement
    IntervalValue offset = as.getByteOffset(gep);
    // Get the new addresses with the offset applied
    as[lhs] = as.getGepObjAddrs(rhs, offset);
}


void AbstractExecution::updateStateOnPhi(const PhiStmt *phi)
{
    const ICFGNode *icfgNode = phi->getICFGNode();
    AbstractState &as = getAbsStateFromTrace(icfgNode);
    u32_t res = phi->getResID();
    auto rhs = AbstractValue();
    // Phi node: join values from all incoming edges
    for (u32_t i = 0; i < phi->getOpVarNum(); i++)
    {
        NodeID opVarId = phi->getOpVarID(i);
        rhs.join_with(as[opVarId]);
    }
    as[res] = rhs;
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
        resVal = lhs + rhs;
    }
    else if (opCode == BinaryOPStmt::Sub || opCode == BinaryOPStmt::FSub)
    {
        resVal = lhs - rhs;
    }
    else if (opCode == BinaryOPStmt::Mul || opCode == BinaryOPStmt::FMul)
    {
        resVal = lhs * rhs;
    }
    else if (opCode == BinaryOPStmt::SDiv || opCode == BinaryOPStmt::FDiv || opCode == BinaryOPStmt::UDiv)
    {
        resVal = lhs / rhs;
    }
    else if (opCode == BinaryOPStmt::SRem || opCode == BinaryOPStmt::FRem || opCode == BinaryOPStmt::URem)
    {
        resVal = lhs % rhs;
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
        // Save the previous state of the cycle head before merging
        AbstractState prevState = preAbsTrace[cycle_head];

        // Merge states from all predecessors of the cycle head
        if (!mergeStatesFromPredecessors(cycle_head, preAbsTrace[cycle_head]))
        {
            // If no feasible incoming state, exit
            break;
        }

        // Copy pre-state to post-state
        postAbsTrace[cycle_head] = preAbsTrace[cycle_head];

        // Process all statements in the cycle head node
        for (const SVFStmt *stmt : cycle_head->getSVFStmts())
        {
            updateAbsState(stmt);
        }

        // Handle call sites if the cycle head is a call node
        if (const CallICFGNode *callnode = SVFUtil::dyn_cast<CallICFGNode>(cycle_head))
        {
            handleCallSite(callnode);
        }

        // Process the cycle body (all components inside the cycle)
        handleWTOComponents(cycle->getWTOComponents());

        // Check for fixpoint: if the state hasn't changed, we've reached convergence
        if (preAbsTrace[cycle_head].equals(prevState))
        {
            break;
        }

        // Apply widening to ensure termination
        // After a certain number of iterations, we need to widen to guarantee convergence
        if (increasing && cur_iter >= Options::WidenDelay())
        {
            preAbsTrace[cycle_head] = preAbsTrace[cycle_head].widening(prevState);
        }
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