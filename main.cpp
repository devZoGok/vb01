#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cameraTest.h"
#include "nodeTest.h"
#include "boneTest.h"
#include "vbModelReaderTest.h"

using namespace CppUnit;
using namespace vb01;

int main(){
	TextUi::TestRunner runner;
	runner.addTest(CameraTest::suite());
	runner.addTest(NodeTest::suite());
	runner.addTest(BoneTest::suite());
	runner.addTest(VbModelReaderTest::suite());
	runner.run();
	return 0;
}
