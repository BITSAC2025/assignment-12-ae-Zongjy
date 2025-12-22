/**
 * Z3Mgr.h
 * @author kisslune 
 */

#ifndef ANSWERS_DEV_Z3MGR_H
#define ANSWERS_DEV_Z3MGR_H

#include "z3++.h"
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace SVF
{

#ifdef DEBUGINFO
#	define DBOP(X) X;
#else
#	define DBOP(X)
#endif

// an ObjVar's ID in SVFIR will be marked using AddressMask (0x7f000000) to mimic the virtual memory address.
// Note that this is not a physical runtime address but an abstract address to easy debugging purposes
// FlippedAddressMask is used to strip off the mask
#define AddressMask 0x7f000000
#define FlippedAddressMask (AddressMask ^ 0xffffffff)

typedef unsigned u32_t;
typedef signed s32_t;

/// Z3 manager interface
class Z3Mgr
{
public:
    /// Constructor
    Z3Mgr(u32_t numOfMapElems)
            : solver(ctx), varID2ExprMap(ctx), lastSlot(numOfMapElems)
    {
        resetZ3ExprMap();
    }

    /// reset and reinitialize Z3Exprs.
    /// varID2ExprMap: maps a variable id to its z3 expression
    /// loc2ValMap: maps an address location to its stored value, e.g., loc2ValMap[addr] = val
    inline void resetZ3ExprMap()
    {
        varID2ExprMap.resize(lastSlot + 1);
        z3::expr loc2ValMap = ctx.constant("loc2ValMap", ctx.array_sort(ctx.int_sort(), ctx.int_sort()));
        updateZ3Expr(lastSlot, loc2ValMap);
    }

    /// Store and Select for Loc2ValMap, i.e., store and load
    z3::expr storeValue(const z3::expr loc, const z3::expr value);
    z3::expr loadValue(const z3::expr loc);

    /// The physical address starts with 0x7f...... + idx
    inline u32_t getVirtualMemAddress(u32_t idx) const
    {
        return AddressMask + idx;
    }

    inline bool isVirtualMemAddress(u32_t val)
    {
        return (val > 0 && (val & AddressMask) == AddressMask);
        // return ((val & AddressMask) > 0);
    }

    inline bool isVirtualMemAddress(z3::expr e)
    {
        z3::expr val = getEvalExpr(e);
        if (val.is_numeral())
        {
            return isVirtualMemAddress(z3Expr2NumValue(val));
        }
        else
        {
            return false;
        }
    }

    /// Return the internal index if idx is an address otherwise return the value of idx
    inline u32_t getInternalID(u32_t idx) const
    {
        return (idx & FlippedAddressMask);
    }

    /// Return Z3 expression based on SVFVar ID
    inline z3::expr getZ3Expr(u32_t idx) const
    {
        assert(getInternalID(idx) == idx && "SVFVar idx overflow > 0x7f000000?");
        assert(varID2ExprMap.size() >= idx + 1 && "idx out of bound for map access, increase map size!");
        return varID2ExprMap[getInternalID(idx)];
    }

    /// Update expression when assignments
    inline void updateZ3Expr(u32_t idx, z3::expr target)
    {
        assert(varID2ExprMap.size() >= idx + 1 && "idx out of bound for map access, increase map size!");
        varID2ExprMap.set(getInternalID(idx), target);
    }

    /// Return int value from an expression if it is a numeral, otherwise return an approximate value
    s32_t z3Expr2NumValue(z3::expr e);

    /// It checks if the constraints added to the Z3 solver are satisfiable.
    /// If they are, it retrieves the model that satisfies these constraints
    /// and evaluates the given complex expression e within this model, returning the evaluated result
    z3::expr getEvalExpr(z3::expr e);

    /// Print all expressions' values after evaluation
    void printExprValues();

    // Print all Z3 expressions
    void printZ3Exprs();

    /// Return the z3 solver
    inline z3::solver &getSolver()
    {
        return solver;
    }

    /// Return the z3 solver context (typically corresponding to a program)
    inline z3::context &getCtx()
    {
        return ctx;
    }

    // Clean up the maps
    inline void clearVarID2ExprMap()
    {
        while (!varID2ExprMap.empty())
            varID2ExprMap.pop_back();

        resetZ3ExprMap();
    }

    // For assert (Q), add ¬Q to the solver to prove the absence of counterexamples;
    // return true means there is no counterexample, false means there is at least one counterexample
    bool checkNegateAssert(z3::expr q)
    {
        // negative check
        getSolver().push();
        getSolver().add(!q);
        getSolver().check();
        bool res = getSolver().check() == z3::unsat;
        getSolver().pop();
        return res;
    }

public:
    z3::context ctx;
    z3::solver solver;

private:
    z3::expr_vector varID2ExprMap;    /// var to z3 expression
    u32_t lastSlot;        /// the last slot in the map for the z3 expression.
};


class Z3Tests : public Z3Mgr
{
public:
    Z3Tests(u32_t max = 128)
            : Z3Mgr(max), maxNumOfExpr(max), currentExprIdx(0)
    {}

    // Return an z3 expr given an id
    inline z3::expr getZ3Expr(u32_t val)
    {
        return ctx.int_val(val);
    }

    // Return true if strToIDMap has this expression name
    inline bool hasZ3Expr(std::string exprName)
    {
        return strToIDMap.find(exprName) != strToIDMap.end();
    }

    // Return an z3 expr given a string name
    inline z3::expr getZ3Expr(std::string exprName)
    {
        auto it = strToIDMap.find(exprName);
        if (it != strToIDMap.end())
            return Z3Mgr::getZ3Expr(it->second);
        else
        {
            strToIDMap[exprName] = ++currentExprIdx;
            assert(maxNumOfExpr >= currentExprIdx && "creating more expression than upper limit");
            z3::expr e = ctx.int_const(exprName.c_str());
            updateZ3Expr(currentExprIdx, e);
            return e;
        }
    }

    // Return an z3 expr for an object given a string name
    inline z3::expr getMemObjAddress(std::string exprName)
    {
        z3::expr e = getZ3Expr(exprName);
        auto iter = strToIDMap.find(exprName);
        assert(iter != strToIDMap.end() && "address expr not found?");
        e = getZ3Expr(Z3Mgr::getVirtualMemAddress(iter->second));
        updateZ3Expr(iter->second, e);
        return e;
    }

    // Return a field object or array element object given a base pointer and an offset
    inline z3::expr getGepObjAddress(z3::expr pointer, u32_t offset)
    {
        std::string baseObjName = pointer.to_string();
        auto iter = strToIDMap.find(baseObjName);
        assert(iter != strToIDMap.end() && "Gep BaseObject expr not found?");
        u32_t baseObjID = iter->second;
        u32_t gepObj = baseObjID + offset;
        if (baseObjID == gepObj)
        {
            return pointer;
        }
        else
        {
            gepObj += maxNumOfExpr / 2; // gep obj ID starts from maxNumOfExpr/2 in this lab exercise
            z3::expr e = getZ3Expr(Z3Mgr::getVirtualMemAddress(gepObj));
            updateZ3Expr(gepObj, e);
            return e;
        }
    }

    // Add an z3 expression into solver for later satisfiability solving
    void addToSolver(z3::expr e)
    {
        solver.add(e);
    }

    // Reset solver's stack and clear up the maps
    void resetSolver()
    {
        solver.reset();
        strToIDMap.clear();
        currentExprIdx = 0;
        clearVarID2ExprMap();
    }

    /// Print out all expressions' values after evaluation
    void printExprValues()
    {
        std::cout.flags(std::ios::left);
        std::cout << "-----------Var and Value-----------\n";
        for (auto nIter = strToIDMap.begin(); nIter != strToIDMap.end(); nIter++)
        {
            z3::expr e = Z3Mgr::getEvalExpr(Z3Mgr::getZ3Expr(nIter->second));
            if (e.is_numeral())
            {
                s32_t value = e.get_numeral_int64();
                std::stringstream exprName;
                exprName << "Var" << nIter->second << " (" << nIter->first << ")";
                std::cout << std::setw(25) << exprName.str();
                if (Z3Mgr::isVirtualMemAddress(value))
                    std::cout << "\t Value: " << std::hex << "0x" << value << "\n";
                else
                    std::cout << "\t Value: " << std::dec << value << "\n";
            }
        }
        std::cout << "-----------------------------------------\n";
    }

    /// To implement
    ///@{
    void test0();
    void test1();
    void test2();
    void test3();
    void test4();
    void test5();
    void test6();
    void test7();
    void test8();
    void test9();
    void test10();
    ///@}

private:
    std::map<std::string, u32_t> strToIDMap;    /// map a string name to its corresponding id (only used in Lab-Exercise-2)
    u32_t maxNumOfExpr;
    u32_t currentExprIdx;
};

} // namespace SVF

#endif //ANSWERS_DEV_Z3MGR_H
