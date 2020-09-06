#ifndef CAMERA_TEST_H
#define CAMERA_TEST_H

#include<cppunit/TestFixture.h>
#include"camera.h"
#include <cppunit/extensions/HelperMacros.h>

namespace CppUnit{
	class Test;
}

namespace vb01{
	class CameraTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE( CameraTest );
		CPPUNIT_TEST( testLookAt );
		CPPUNIT_TEST_SUITE_END();
		public:
			CameraTest();
			void setUp();
			void tearDown();
			void testLookAt();
		private:
			Camera *cam = nullptr;
	};
}

#endif
