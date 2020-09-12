#ifndef BONE_TEST_H
#define BONE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Model;
	class Node;

	class BoneTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(BoneTest);
		CPPUNIT_TEST(testGetModelSpacePos);
		CPPUNIT_TEST_SUITE_END();

		public:
			BoneTest();
			void setUp();
			void tearDown();
		private:
			void testGetModelSpacePos();

			Model *model = nullptr;
			Node *rootNode = nullptr;
	};
}

#endif
