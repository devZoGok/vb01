#include "cameraTest.h"

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>

using namespace CppUnit;

namespace vb01{
	CameraTest::CameraTest(){
	}

	void CameraTest::setUp(){
		cam = new Camera();
	}

	void CameraTest::tearDown(){
		delete cam;
	}

	void CameraTest::testLookAt(){
		cam->lookAt(Vector3(0, 0, -1));
		CPPUNIT_ASSERT(cam->getLeft() == Vector3(1, 0, 0));
	}
}
