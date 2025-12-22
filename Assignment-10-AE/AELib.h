/**
 * AELib.h
 * @author kisslune 
 */

#ifndef ANSWERS_AELIB_H
#define ANSWERS_AELIB_H

#include "AEHelper.h"
#include "AE/Svfexe/AbsExtAPI.h"
#include "SVFIR/SVFIR.h"

namespace SVF
{
/// Abstract Execution class
class AbstractExecution
{
public:
    /// Constructor
    AbstractExecution()
    {
    }

    virtual void runOnModule(ICFG *icfg);

    static AbstractExecution &getAEInstance()
    {
        static AbstractExecution instance;
        return instance;
    }

    /// Handle global variables and initializations
    void handleGlobalNode();

    /// Driver of the program
    virtual void analyse();

    void handleWTOComponents(const std::list<const ICFGWTOComp*>& wtoComps);
    void handleSingletonWTO(const ICFGSingletonWTO *icfgSingletonWto);
    void handleCycleWTO(const ICFGCycleWTO* cycle);

    /// Handle state updates for each type of SVF statement
    virtual void updateAbsState(const SVFStmt *stmt);

    // handle SVF Statements
    ///@{
    void updateStateOnAddr(const AddrStmt *addr);
    void updateStateOnGep(const GepStmt *gep);
    void updateStateOnStore(const StoreStmt *store);
    void updateStateOnLoad(const LoadStmt *load);
    void updateStateOnCmp(const CmpStmt *cmp);
    void updateStateOnCall(const CallPE *call);
    void updateStateOnRet(const RetPE *retPE);
    void updateStateOnCopy(const CopyStmt *copy);
    void updateStateOnPhi(const PhiStmt *phi);
    void updateStateOnBinary(const BinaryOPStmt *binary);
    void updateStateOnSelect(const SelectStmt *select);
    ///@}

    /// Handle stub functions for verifying abstract interpretation results
    void handleStubFunctions(const CallICFGNode *call);

    /// Mark recursive functions in the call graph
    void initWTO();

    /// Path feasiblity handling
    ///@{
    bool mergeStatesFromPredecessors(const ICFGNode *curNode, AbstractState &as);

    bool isCmpBranchFeasible(const CmpStmt *cmpStmt, s64_t succ, AbstractState &as);
    bool isSwitchBranchFeasible(const SVFVar *var, s64_t succ, AbstractState &as);
    bool isBranchFeasible(const IntraCFGEdge *intraEdge, AbstractState &as);
    ///@}

    /// Handle a call site in the control flow graph
    void handleCallSite(const CallICFGNode *callnode);
    bool isExternalCallForAssignment(const SVF::FunObjVar *func);

    /// Handle a function in the ICFG
    void handleFunction(const CallICFGNode *callNode);

    /// Get the next nodes of a node
    std::vector<const ICFGNode *> getNextNodes(const ICFGNode *node) const;

    /// Return its abstract state given an ICFGNode
    AbstractState &getAbsStateFromTrace(const ICFGNode *node)
    {
        return postAbsTrace[node];
    }

    void ensureAllAssertsValidated();

    /// Destructor
    virtual ~AbstractExecution()
    {
    }

protected:
    /// SVFIR and ICFG
    SVFIR *svfir;
    ICFG *icfg;

    /// Map a function to its corresponding WTO
    Map<const FunObjVar *, ICFGWTO *> funcToWTO;
    /// A set of functions which are involved in recursions
    Set<const FunObjVar *> recursiveFuns;
    /// Abstract trace immediately before an ICFGNode.
    Map<const ICFGNode *, AbstractState> preAbsTrace;
    /// Abstract trace immediately after an ICFGNode.
    Map<const ICFGNode *, AbstractState> postAbsTrace;

private:
    AbstractExecutionHelper bufOverflowHelper;

    Set<const CallICFGNode *> assert_points;

    Map<const ICFGNode *, const ICFGCycleWTO *> cycleHeadToCycle;

    AbsExtAPI *utils;
};

} // namespace SVF

#endif //ANSWERS_AELIB_H
