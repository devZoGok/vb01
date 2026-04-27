#ifndef ANIMATION_CHANNEL_TEST
#define ANIMATION_CHANNEL_TES

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Skeleton;
	class AnimationChannel;

	class AnimationChannelTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(AnimationChannelTest);
		CPPUNIT_TEST(testUpdate);
		CPPUNIT_TEST(testGetMaxKeyframeNum);
		CPPUNIT_TEST(testGetFirstFrame);
		CPPUNIT_TEST_SUITE_END();
	
		public:
			AnimationChannelTest(){}
			~AnimationChannelTest(){}
			void setUp();
			void tearDown();
		private:
			Skeleton *skeleton = nullptr;
			AnimationChannel *channel = nullptr;

			void testUpdate();
			void testGetMaxKeyframeNum();
			void testGetFirstFrame();
	};
}


#endif
