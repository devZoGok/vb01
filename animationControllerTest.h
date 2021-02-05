#ifndef ANIMATION_CONTROLLER_TEST_H
#define ANIMATION_CONTROLLER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Skeleton;
	class AnimationChannel;
	class AnimationController;

	class AnimationControllerTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(AnimationControllerTest);
		CPPUNIT_TEST(testPrepareAdjacentValues);
		CPPUNIT_TEST_SUITE_END();
		public:
			AnimationControllerTest(){}
			void setUp();
			void tearDown();
		private:
			void createAnimation();
			void testPrepareAdjacentValues();
			
			Skeleton *skeleton = nullptr;
			AnimationController *controller = nullptr;
			AnimationChannel *channel = nullptr;
	};
}

#endif
