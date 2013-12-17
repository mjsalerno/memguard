#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>

#include "memoryalloc.h"

using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class UnitTest : public CppUnit::TestFixture {

    CPPUNIT_TEST_SUITE(UnitTest);
    CPPUNIT_TEST(memoryAlloc);
    CPPUNIT_TEST(testMultiply);
    CPPUNIT_TEST_SUITE_END();

    char *string;
    char *s2;

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testMultiply(void);
    void memoryAlloc(void);

};

//-----------------------------------------------------------------------------

void UnitTest::memoryAlloc(void) {
    MemoryAlloc ma1(string, 56);
    MemoryAlloc ma2(s2, 102);
    // Check to make sure the correct values are being returned
    CPPUNIT_ASSERT(ma1.getUserSize() == 56);
    CPPUNIT_ASSERT(ma2.getUserSize() == 102);
    CPPUNIT_ASSERT(ma1.getUserSize() != ma2.getUserSize());
    //cout << "=== MemoryAlloc getSize() test passed." << endl;
    CPPUNIT_ASSERT(ma1.getAddress() == (char*)string + DEFAULT_FENCE_SIZE);
    CPPUNIT_ASSERT(ma2.getAddress() != (char*)string + DEFAULT_FENCE_SIZE);
    CPPUNIT_ASSERT(ma1.getAddress() != ma2.getAddress());
    CPPUNIT_ASSERT(ma2.getAddress() == (char*)s2 + DEFAULT_FENCE_SIZE);
    //cout << "=== MemoryAlloc getAddress() test passed." << endl;
    // Test The print feature
    //cout << "=== Testing the print function" << endl;
    ma1.print();
    ma2.print();
    // Free allocated memory
    free(string);
    free(s2);
}

void UnitTest::testMultiply(void) {
    CPPUNIT_ASSERT(6 == 2*3);
}

void UnitTest::setUp(void) {
    string = (char*) malloc(56);
    s2 = (char*) malloc(102);
}

void UnitTest::tearDown(void) {
    free(string);
    free(s2);
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( UnitTest );

int main(int argc, char* argv[]) {
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("unitTest.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}
          