#### We describe the API of `Z3Mgr` and its subclasses `Z3ExampleMgr` (used in Lab-Exercise-2) and `Z3SSEMgr` (used in Assignment-2).


## Z3Mgr (Z3 Manager)
|Members|Meanings|
|------|------------------------------------------|
|SVF:: Z3Mgr::storeValue(const z3::expr loc, const z3::expr value) | store `value` to the location `loc` in `loc2ValMap` (which is a Z3 array for handling memory operations)
|SVF:: Z3Mgr::loadValue(const z3::expr loc) | retrieve the value at location `loc` in `loc2ValMap`|
|SVF:: Z3Mgr::getZ3Expr(u32_t idx)| return the Z3 expression based on the ID of an SVFVar|
|SVF:: Z3Mgr::updateZ3Expr(u32_t idx, z3::expr target)| update expression for an SVFVar given its ID|
|SVF:: Z3Mgr::getEvalExpr(z3::expr e) | return evaluated value of an expression if the expression has a solution (model); asserts unsat otherwise|
|SVF:: Z3Mgr::getSolver | return the Z3 solver|
|SVF:: Z3Mgr::getCtx | return the Z3 context|
|SVF:: Z3Mgr::resetZ3ExprMap | reset added expressions and clean all declared values|
|solver.push() | creates a new scope by saving the current stack |
|solver.pop() | pop removes any expressions performed between it and the matching push|
|checkNegateAssert | For assert (Q), add ¬Q to the solver to prove the absence of counterexamples; It returns true if it is the absence of counterexamples, otherwise it has at least one counterexample|


## Z3ETests
|Members|Meanings|
|------|------------------------------------------|
|SVF::Z3ExampleMgr::getZ3Expr(u32_t val)| return the Z3 expression given a constant integer value (e.g., `getZ3Expr(5)` returns the expression `5`) |
|SVF::Z3ExampleMgr::getZ3Expr(std::string exprName)|return the Z3 expression based on a variable's name|
|SVF::Z3ExampleMgr::getMemObjAddress(std::string exprName) | return the virtual memory address based on an object's name |
|SVF::Z3ExampleMgr::getGepObjAddress(z3::expr pointer, u32_t offset)| return the virtual memory address of a field given a base pointer and offset |
|SVF::Z3ExampleMgr::addToSolver(z3::expr e)| add a Z3 expression into the solver |
|SVF::Z3ExampleMgr::resetSolver()| reset added expressions and clean all declared values |
|SVF::Z3ExampleMgr::printExprValues()|print the values of all Z3 expressions|



## Z3SSEMgr and SSE
|Members|Meanings|
|------|------------------------------------------|
|SVF:: Z3SSEMgr:: createExprForObjVar(const ObjVar* obj)| initialize the expr value for an object (address-taken variables and constants)|
|SVF:: Z3SSEMgr::getMemObjAddress(u32_t idx) |return the address expr of a ObjVar | 
|SVF:: Z3SSEMgr::getGepObjAddress(z3::expr pointer, u32_t offset)| return the field address given a pointer pointing to a struct object and an offset| 
|SVF:: Z3SSEMgr::getGepOffset(const GepStmt* gep, const CallStack& callingCtx) |return the offset (`u32_t`) given a GepStmt and the current calling context | 
|SVF:: Z3SSEMgr:: printExprValues | dump the values of all expressions added to the solver|
|SVF::Z3SSEMgr::callingCtxToStr| return the calling context in the form of string.|
|SVF::Z3SSEMgr::getZ3Expr(u32_t idx, const CallStack& callingCtx)| return the z3 expression given an SVFVarID and a calling context|
|SVF::SSE::getZ3Expr(u32_t idx)| return the z3 expression given an SVFVarID and the current calling context `callingCtx`|
|SVF::SSE::pushCallingCtx(ICFGNode*)| Add a callsite to the top of the current `callingCtx` stack, and all subsequent retrievals of Z3 expressions via `getZ3Expr` will be affected|
|SVF::SSE::popCallingCtx()| Pop top callsite from `callingCtx` stack|
|SVF::SSE::callingCtx| calling context used (vector of call ICFGNode*) for path translation only|
|SVF::SSE::callstack| calling context used (vector of call ICFGNode*) for reachability analysis only|
|SVF::SSE::getCtx().int_val(u32_t int_literal) |return the z3 expression for int literal (e.g., 0 and 1). **DO NOT** use getZ3Expr(0) or getZ3Expr(1) for int constants in Assignment-2. | 
|SVF::SSE::getEvalExpr(z3::expr e)|  checks if the constraints added to the Z3 solver are satisfiable. If so, evaluates e within this model, returning the evaluated result like bool, int, real and string |
|SVF::Z3Mgr::printZ3Exprs()|print all z3 expressions in the solver without solving or evaluation|
|SVF::SVFValue::getID()| get the VarID given an SVF Value*| 
|SVF::SSE::addToSolver(z3::expr e)| add a Z3 expression into the solver |
|SVF::CallCFGEdge::getCallPEs()| return all CallPEs at the call site (actual-to-formal arguments assignments)|
|SVF::RetCFGEdge::getRetPE()| return the RetPE at the call site (return-to-receiving argument assignment). Note that it can be nullptr if the callee function returns void|

### Z3 C++ API Reference

#### 1️⃣ Creating Z3 Expressions

| API | Description |
|-----|-------------|
| `context` | The main environment for creating and managing expressions and solvers. |
| `expr` | Represents a symbolic expression (e.g., variable, arithmetic, boolean). |
| `context.int_const(name)` | Declares a symbolic integer variable in the context. |
| `context.bool_const(name)` ✳️ | Declares a symbolic boolean variable. |
| `context.real_const(name)` ✳️ | Declares a symbolic real variable. |
| `context.int_val(n)` | Returns a Z3 integer constant expression with value `n`. |

#### 2️⃣ Conditional and Logical Expressions

| API | Description |
|-----|-------------|
| `ite(condition, then_expr, else_expr)` | If-then-else expression (returns `then_expr` if condition is true, else `else_expr`). |
| `operator&&`, `operator||`, `!expr` | Logical AND, OR, and NOT operators for boolean expressions. |
| `expr1 == expr2` | Checks equality between expressions. |
| `expr1 != expr2` ✳️ | Checks inequality between expressions. |
| `expr1 > expr2`, `<`, `>=`, `<=` | Relational operators for integer or real expressions. |

#### 3️⃣ Memory Modeling (Arrays)

| API | Description |
|-----|-------------|
| `context.array_sort(index_sort, element_sort)` | Creates the sort (type) of arrays (used to model memory). |
| `z3::select(array_expr, index_expr)` | Reads the value at a given index from an array (i.e., memory load). |
| `z3::store(array_expr, index_expr, value_expr)` | Updates an array at `index_expr` with `value_expr` (i.e., memory write). |
| `z3::const(name, sort)` | Declares a named constant of a given sort. |

#### 4️⃣ Solver and Model Interaction

| API | Description |
|-----|-------------|
| `z3::solver` | Represents a Z3 solver instance. |
| `solver.add(expr)` | Adds a constraint expression to the solver. |
| `solver.check()` | Checks satisfiability of current constraints. Returns `sat`, `unsat`, or `unknown`. |
| `solver.get_model()` | Returns a satisfying model after `check()`. |
| `model.eval(expr)` | Evaluates an expression under the given model. |

#### 5️⃣ Context and Basic Sorts

| API | Description |
|-----|-------------|
| `z3::context` | Central object for managing all Z3 objects. Needed to construct solvers, expressions, etc. |
| `z3::int_sort(ctx)` | Returns the integer sort. |
| `z3::bool_sort(ctx)` ✳️ | Returns the boolean sort. |
| `z3::real_sort(ctx)` ✳️ | Returns the real number sort. |
| `z3::sat`, `z3::unsat`, `z3::unknown` | Constants representing the result of `solver.check()`. |

 

