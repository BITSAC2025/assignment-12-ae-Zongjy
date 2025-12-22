# Essential API Documentation for Labs and Assignments


## Lab-Exercise-3


| API                                       | Introduction                                                                                          |
|-------------------------------------------|------------------------------------------------------------------------------------------------------|
| `getNodeID("variable")`                   | Retrieves the node ID of the specified variable.                                                      |
| `IntervalValue(lower, upper)`             | Creates an interval value.                                                                            |
| `AbstractValue::getInterval()`                  | Retrieves the interval value of the abstract state.                                                   |
| `AbstractValue::join_with(value)`         | Merges the current value with another value.                                                          |
| `getMemObjAddress("variable")`            | Retrieves the memory object address of the specified variable.                                        |
| `AddressValue(getMemObjAddress("variable"))` | Creates an address value initialized to the memory object address of the specified variable.        |
| `AEState::widening(state)`                | Performs widening on the given state.                                                                 |
| `AEState::narrowing(state)`               | Performs narrowing on the given state.                                                                |
| `AEState::joinWith(state)`                | Merges the current state with another state.                                                          |
| `AbstractValue::meet_with(value)`         | Performs an intersection operation between the current value and another value.                       |
| `getGepObjAddress("variable", offset)`    | Retrieves the GEP (GetElementPtr) object address of the specified variable with the given offset.     |
| `AEState::loadValue(varId)`               | Loads the abstract value from the variable ID's address.                                              |
| `AEState::storeValue(varId, val)`         | Stores the abstract value at the variable ID's address.                                               |
| `AEState::printAbstractState()` | Prints the abstract trace for debugging purposes.                               |

-----

#### `AEState::widening(state)`
- Perform widening on the current state with another state.

    For example,

    ```cpp
    void exampleWidening() {
        AEState as1, as2;
        NodeID a = getNodeID("a");
        as1[a] = IntervalValue(1, 5); // as1: a in [1, 5]
        as2[a] = IntervalValue(3, 10); // as2: a in [3, 10]
        AEState widenedState = as1.widening(as2); // widenedState: a in [1, +inf]
    }
   
    ```

    **Input:** `state` (an AEState)
    
    **Output:** Widened state (an AEState)

-----

#### `AEState::loadValue(varId)` and `AEState::storeValue(varId, val)`
- Load and store values associated with a variable ID.

    For example,

    ```cpp
    void exampleLoadValue() {
        AEState as;
        NodeID a = getNodeID("a");
        NodeID p = getNodeID("p");
        NodeID malloc = getNodeID("malloc");
        as[p] = AddressValue(getMemObjAddress("malloc"));
        as.storeValue(p, IntervalValue(42, 42)); // Store 42 at address p

        AbstractValue loadedValue = as.loadValue(p);

        std::cout << loadedValue.toString() << std::endl;
    }

    ```

    **Input:** `varId` (a NodeID), `val` (an AbstractValue)
    
    **Output:** Loaded value (an AbstractValue, can be Interval Value or Address Value)

-----

#### `AEState::printAbstractState()`
- Print the abstract trace for debugging purposes.

    For example,

    ```cpp

    void examplePrintAbstractState() {
        AEState as;
        NodeID a = getNodeID("a");
        NodeID b = getNodeID("b");

        as[a] = IntervalValue(1, 5); // a in [1, 5]
        as[b] = IntervalValue(3, 7); // b in [3, 7]

        as.printAbstractState(); // Print the abstract trace for debugging
    }

    ```

    **Input:** None
    
    **Output:** None (prints the abstract trace)

```
-----------Var and Value-----------
Var2(b)             :  Value: [3, 7]
Var1(a)             :  Value: [1, 5]
-----------------------------------------
```

-----

## Assignment-3

| API                                       | Introduction                                                                    |
|-------------------------------------------|---------------------------------------------------------------------------------|
| `SVFUtil::isa<CallICFGNode>(stmt)`        | Checks if the given statement is a call ICFG node.                              |
| `SVFUtil::dyn_cast<GepStmt>(stmt)`        | Dynamically casts the given statement to a `GepStmt` type.                      |
| `AbstractExecution::getAbsStateFromTrace(node)`    | Retrieves the abstract state of the given ICFG node.                            |
| `GepStmt::getLHSVarID()`                  | Retrieves the left-hand side variable ID of the `GepStmt`.                      |
| `GepStmt::getRHSVarID()`                  | Retrieves the right-hand side variable ID of the `GepStmt`.                     |
| `AEState::getAddrs()`                     | Retrieves the address values in the abstract state.                             |
| `AEState::getIDFromAddr(addr)`            | Retrieves the internal ID of the given address.                                 |
| `svfir->getBaseObject(id)->getByteSizeOfObj()` | Retrieves the byte size of the base object.                                     |
| `Options::WidenDelay()`                   | Retrieves the value of the widen delay option.                                  |
| `AEState::widening(state)`                | Performs widening on the given state.                                           |
| `AEState::narrowing(state)`               | Performs narrowing on the given state.                                          |
| `handleICFGNode(ICFGNode*)`               | Handles a singleton WTO (Weak Topological Order) which includes an ICFGNode.    |
| `handleICFGCycle(ICFGCycleWTO*)`          | Handles WTO cycles, which includes a set of ICFGNode.             |
| `ICFGCycleWTO::head()::getICFGNode()`     | Get the cycle head ICFGNode             |
| `mergeStatesFromPredecessors(node, state)`| Merges states from predecessor ICFGNodes.                                       |
| `CopyStmt::getLHSVarID()`                 | Retrieves the left-hand side variable ID of the copy statement.                 |
| `CopyStmt::getRHSVarID()`                 | Retrieves the right-hand side variable ID of the copy statement.                |
| `BinaryOPStmt::getOpVarID(index)`         | Retrieves the operand variable ID of the binary operation statement.            |
| `BinaryOPStmt::getResID()`                | Retrieves the result variable ID of the binary operation statement.             |
| `BinaryOPStmt::getOpcode()`               | Retrieves the opcode of the binary operation statement.                         |
| `AEState::loadValue(varId)`               | Loads the abstract value of the given variable ID.                              |
| `AEState::storeValue(varId, val)`         | Stores the abstract value at the given variable ID.                             |
| `LoadStmt::getRHSVarID()`                 | Retrieves the right-hand side variable ID of the load statement.                |
| `LoadStmt::getLHSVarID()`                 | Retrieves the left-hand side variable ID of the load statement.                 |
| `IntervalValue::getIntNumeral()`          | Returns the integer representation of the interval value.                       |
| `AEState::getByteOffset(gep)`             | Retrieves the byte offset of the GEP statement.                                 |
| `AEState::getElementIndex(gep)`           | Retrieves the element index of the GEP statement.                               |
| `AEState::getGepObjAddrs(varID, offset)`  | Retrieves the address of the GEP object.                                        |
| `AbstractExecution::reportBufOverflow(node)` | Reports a buffer overflow for a given ICFG node.                               |
| `AbstractExecution::updateGepObjOffsetFromBase(gepAddrs, objAddrs, offset)` | Updates the GEP object offset from the base.    |
| `AbstractExecution::getAccessOffset(objId, gep)` | Returns the accessing offset of an object at a GepStmt.                        |
| `AbstractExecution::updateStateOnExtCall(extCallNode)` | Handles external calls, checking for buffer overflows, and updates abstract states using memcopy-like APIs via AbsExtAPI::handleMemcpy                    |
| `AbsExtAPI::handleMemcpy(as, dst, src, len, start_idx)` | Simulates a memcpy operation in the abstract state as: copies `len` bytes from the source variable `src` to the destination variable `dst`, starting at offset `start_idx`. The function automatically determines the element size (for arrays or pointers) and performs element-wise copying, updating the abstract state accordingly. Only objects present in the abstract state are copied. |
| `AEState::getIDFromAddr(addr)`                           | Retrieves the internal ID associated with a given address.                    |
| `bufOverflowHelper.addToGepObjOffsetFromBase(objVar, offset)` | Adds an offset to the GEP object offset from the base object.            |
| `bufOverflowHelper.hasGepObjOffsetFromBase(objVar)`      | Checks if there is a GEP object offset from the base object.                  |
| `bufOverflowHelper.getGepObjOffsetFromBase(objVar)`      | Retrieves the GEP object offset from the base object.                         |
| `AEState::getByteOffset(gep)`                            | Retrieves the byte offset for a given GEP instruction.                        |
| `AEState::getGepObjAddrs(varID, offset)`                 | Gets the addresses of GEP objects given a variable ID and an offset.          |
| `AbstractExecution::updateGepObjOffsetFromBase(gepAddrs, objAddrs, offset)` | Updates the GEP object offset from the base object.               |
| `AbstractExecution::getAccessOffset(objId, gep)`         | Retrieves the access offset for a given object ID and GEP instruction.        |
| `AEState::printAbstractState()`                          | Prints the abstract trace of the execution.                                   |

<!-- utils->handleMemcpy(as, extCallNode->getArgument(0), extCallNode->getArgument(1), len,  as[position_id].getInterval().getIntNumeral());

void AbsExtAPI::handleMemcpy(AbstractState& as, const SVF::SVFVar *dst, const SVF::SVFVar *src, IntervalValue len,  u32_t start_idx) -->

----
#### AEState::getIDFromAddr(addr)
- Retrieve the internal ID associated with a given address.

    For example,

    ```cpp
    // Assuming 'addr' is a valid address object
    int internalID = AEState::getIDFromAddr(addr);
    std::cout << "Internal ID: " << internalID << std::endl;
    ```

    **Input:** `addr` (an address object starting with 0x7f****), e.g. 0x7f0005
    
    **Output:** Internal ID associated with `addr` (remove the mask of 0x7f0000), e.g. 5 (0x7f0005 remove the mask)

-----

#### bufOverflowHelper.addToGepObjOffsetFromBase(objVar, offset)
- Add an offset to the GEP object offset from the base object.

    For example,

    ```cpp
    // Assuming 'GepObjVar' is a valid Gep object variable and 'offset' is an integer
    bufOverflowHelper.addToGepObjOffsetFromBase(gepObjVar, offset);
    ```

    **Input:** `gepObjVar` (a gep object variable), `offset` (an Interval Value)
    
    **Output:** None (operation performed)

-----

#### bufOverflowHelper.hasGepObjOffsetFromBase(objVar)
- Check if there is a GEP object offset from the base object.

    For example,

    ```cpp
    // Assuming 'objVar' is a valid object variable
    bool hasOffset = bufOverflowHelper.hasGepObjOffsetFromBase(gepObjVar);
    ```

    **Input:** `gepObjVar` (a gep object variable)
    
    **Output:** Boolean indicating if there is a GEP object offset from the base

-----

#### bufOverflowHelper.getGepObjOffsetFromBase(objVar)
- Retrieve the GEP object offset from the base object.

    For example,

    ```cpp
    // Assuming 'objVar' is a valid object variable
    IntervalValue accessOffset = _bufOverflowHelper.getGepObjOffsetFromBase(objVar);
    std::cout << "GEP object offset from base: " << accessOffset.toString() << std::endl;
    ```

    **Input:** `gepObjVar` (a gep object variable)
    
    **Output:** Interval value representing the GEP object offset from the base

-----

#### svfir->getBaseObject(id)->getByteSizeOfObj()
- Get the byte size of a base object given its ID.

    For example,

    ```cpp
    // Assuming 'id' is a valid object ID
    u32_t byteSize = svfir->getBaseObject(id)->getByteSizeOfObj();
    std::cout << "Byte size of base object: " << byteSize << std::endl;
    ```

    **Input:** `id` (an object ID, either GepObj or BaseObj)
    
    **Output:** Unsigned integer representing the byte size of the base object

-----

#### AEState::getByteOffset(gep)
- Retrieve the byte offset (has lower and upper bound) for a given GEP instruction.

    For example,

    ```cpp
    // Assuming 'gep' is a valid GEP instruction
    IntervalValue byteOffset = as.getByteOffset(gep);
    std::cout << "Byte offset: " << byteOffset.toString() << std::endl;
    ```

    **Input:** `gep` (a GEP instruction)
    
    **Output:** IntervalValue representing the byte offset

-----

#### AEState::getGepObjAddrs(varID, offset)
- Get the addresses of GEP objects given a variable ID and an offset.

    For example,

    ```cpp
    // Assuming 'varID' is a valid variable ID and 'offset' is an integer
    auto gepObjAddrs = as.getGepObjAddrs(varID, offset);
    std::cout << "GEP object addresses: ";
    for (const auto& addr : gepObjAddrs) {
        std::cout << "0x" << std::hex << addr << ", ";
    }
    std::cout << std::endl;
    ```

    **Input:** `varID` (a variable ID), `offset` (an Interval Value)
    
    **Output:** List of addresses of GEP objects

-----

#### AbstractExecution::updateGepObjOffsetFromBase(gepAddrs, objAddrs, offset)
- Update the GEP object offset from the base object.

    For example,

    ```cpp
    // Assuming 'gepAddrs' and 'objAddrs' are valid address lists and 'offset' is an integer
    AbstractExecution::updateGepObjOffsetFromBase(gepAddrs, objAddrs, offset);
    ```

    **Input:** `gepAddrs` (list of GEP addresses), `objAddrs` (list of object addresses), `offset` (an IntervalValue)
    
    **Output:** None (operation performed). For example, for $gepObj = getelementptr $obj, 20. gepAddrs is the addresses of $gepObj, objAddrsis the addresses of $obj, 20 is the offset (if it is concrete value, the lower bound and the upper bound are both equal to 20.)

-----

#### AbstractExecution::getAccessOffset(objId, gep)
- Retrieve the access offset for a given object ID and GEP instruction.

    For example,

    ```cpp
    // Assuming 'objId' is a valid object ID and 'gep' is a valid GEP instruction
    IntervalValue accessOffset = AbstractExecution::getAccessOffset(objId, gep);
    std::cout << "Access offset: " << accessOffset.toString() << std::endl;
    ```

    **Input:** `objId` (an object ID), `gep` (a GEP instruction)
    
    **Output:** IntervalValue representing the access offset

----

#### AbsExtAPI::handleMemcpy(as, dst, src, len, start_idx)

- Simulates a memcpy operation in the abstract state as: copies `len` bytes from the source variable `src` to the destination variable `dst`, starting at offset `start_idx`. The function automatically determines the element size (for arrays or pointers) and performs element-wise copying, updating the abstract state accordingly. Only objects present in the abstract state are copied.

    For example

    ```cpp
    // as is AbstractState
    // dst is a SVFVar*, whose type can be Array or Pointer.
    // src is a SVFVar*, whose type can be Array or Pointer.
    // len is a IntervalValue, we copy the lower bound bytes
    // start_idx is the starting position
    // e.g. in source code,  char dst[10] = {'a','a','a','a','a','a','a','a','a','a'};
    //                       chat src[5] = {'1', '2', '3', '4', '\0'};
    // we call handleMemcpy(as, dst, src, [5, 5], 2);
    // AbstractState should be updated as `dst = {'a', 'a', '1', '2', '3', '4','\0', 'a','a','a'}`
    // in which dst[2] = '1', dst[3] = '2',dst[4] = '3', dst[5] = '4', dst[6] = '\0', and no change occurs in other indices.

    utils->handleMemcpy(as, dst, src, len, start_idx);

    ```

-----

#### AEState::printAbstractState()
- Print the abstract trace of the execution.

    For example,

    ```cpp
    // Assuming `AEState` is an Abstract Trace
    AEState& as = getAbsStateFromTrace(icfgNode);
    as.printAbstractState();
    ```

    **Input:** None
    
    **Output:** Prints the abstract trace to the standard output, e.g.

```
-----------Var and Value-----------
Var4                 Value: [5, 5]
Var2                 Value: [5, 5]
Var1                 Value: {0x7f000005}
0x7f000005           Value: [5, 5]
——————————————---------------------
```


-----
