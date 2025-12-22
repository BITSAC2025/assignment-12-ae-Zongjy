/**
 * AETest.cpp
 * @author kisslune 
 */

#include "SVF-LLVM/SVFIRBuilder.h"
#include "Util/CommandLine.h"
#include "Util/Options.h"
#include "WPA/WPAPass.h"

#include "AEMgr.h"

using namespace SVF;
using namespace SVFUtil;

int main(int argc, char **argv)
{

    AbstractExecutionMgr &mgr = AbstractExecutionMgr::getInstance();
    AEState as;

    SVFUtil::outs() << "Test 0: \n";
    as = mgr.test0();
    mgr.svf_assert(as[mgr.getNodeID("x")].getInterval() == IntervalValue(5, 5));
    mgr.reset();

    SVFUtil::outs() << "Test 1: \n";
    as = mgr.test1();
    mgr.svf_assert(as[mgr.getNodeID("b")].getInterval() > IntervalValue(0, 0));
    mgr.reset();

    SVFUtil::outs() << "Test 2: \n";
    as = mgr.test2();
    mgr.svf_assert(as[mgr.getNodeID("b")].getInterval() > IntervalValue(3, 3));
    mgr.reset();

    SVFUtil::outs() << "Test 3: \n";
    as = mgr.test3();
    mgr.svf_assert(as[mgr.getNodeID("x")].getInterval() == IntervalValue(10, 10));
    mgr.reset();

    SVFUtil::outs() << "Test 4: \n";
    as = mgr.test4();
    mgr.svf_assert(as[mgr.getNodeID("a")].getInterval() + as[mgr.getNodeID("b")].getInterval()
                   > IntervalValue(20, 20));
    mgr.reset();

    SVFUtil::outs() << "Test 5: \n";
    as = mgr.test5();
    mgr.svf_assert(as[mgr.getNodeID("z")].getInterval() == IntervalValue(15, 15));
    mgr.reset();

    SVFUtil::outs() << "Test 6: \n";
    as = mgr.test6();
    mgr.svf_assert(as[mgr.getNodeID("b")].getInterval() >= IntervalValue(5, 5));
    mgr.reset();

    SVFUtil::outs() << "Test 7: \n";
    as = mgr.test7();
    mgr.svf_assert(as[mgr.getNodeID("x")].getInterval() == IntervalValue(3, 3));
    mgr.svf_assert(as[mgr.getNodeID("y")].getInterval() == IntervalValue(2, 2));
    mgr.reset();

    SVFUtil::outs() << "Test 8: \n";
    as = mgr.test8();
    mgr.svf_assert(as[mgr.getNodeID("x")].getInterval() == IntervalValue(0, 0));
    mgr.reset();

    return 0;
}
