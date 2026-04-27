#ifndef CAMERA_TEST_H
#define CAMERA_TEST_H

#include "camera.h"

#include <cppunit/TestFixture.h>
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
		private:
			void testLookAt();

			Camera *cam = nullptr;
	};
}

#endif
