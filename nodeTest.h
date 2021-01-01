#ifndef NODE_TEST_H
#define NODE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Node;

	class NodeTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(NodeTest);
		CPPUNIT_TEST(testGetAncestors);
		CPPUNIT_TEST(testAdjustDir);
		CPPUNIT_TEST(testAdjustUp);
		CPPUNIT_TEST(testLocalToGlobalPosition);
		CPPUNIT_TEST(testGlobalToLocalPosition);
		CPPUNIT_TEST(testLocalToGlobalOrientation);
		CPPUNIT_TEST(testGlobalToLocalOrientation);
		CPPUNIT_TEST_SUITE_END();

		public:
			NodeTest();
			void setUp();
			void tearDown();
		private:
			void testAdjustDir();
			void testAdjustUp();
			void testDetachChild();
			void testGetAncestors();
			void testLocalToGlobalPosition();
			void testGlobalToLocalPosition();
			void testLocalToGlobalOrientation();
			void testGlobalToLocalOrientation();

			Node *rootNode = nullptr,
				 *ancestorA = nullptr,
				 *ancestorB = nullptr,
				 *ancestorC = nullptr,
				 *ancestorD = nullptr,
				 *lookNodeParent = nullptr,
				 *lookNode = nullptr,
				 *firstChainNode = nullptr,
				 *secondChainNode = nullptr,
				 *thirdChainNode = nullptr;
	};
}

#endif
