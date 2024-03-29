#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "cameraTest.h"
#include "nodeTest.h"
#include "boneTest.h"
#include "animationChannelTest.h"
#include "shaderTest.h"
#include "particleEmitterTest.h"
#include "ikSolverTest.h"
#include "assetManagerTest.h"

using namespace CppUnit;
using namespace vb01;

int main(){
	TextUi::TestRunner runner;
	runner.addTest(CameraTest::suite());
	runner.addTest(NodeTest::suite());
	runner.addTest(BoneTest::suite());
	runner.addTest(AnimationChannelTest::suite());
	runner.addTest(ShaderTest::suite());
	runner.addTest(ParticleEmitterTest::suite());
	runner.addTest(IkSolverTest::suite());
	runner.addTest(AssetManagerTest::suite());
	runner.run();
	return 0;
}
