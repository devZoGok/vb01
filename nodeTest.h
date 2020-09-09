#ifndef NODE_TEST_H
#define NODE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Node;

	class NodeTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(NodeTest);
		//CPPUNIT_TEST(testDetachChild);
		CPPUNIT_TEST(testAdjustDir);
		CPPUNIT_TEST(testAdjustUp);
		CPPUNIT_TEST(testGetDescendants);
		CPPUNIT_TEST_SUITE_END();

		public:
			NodeTest();
			void setUp();
			void tearDown();
		private:
			void testAdjustDir();
			void testAdjustUp();
			void testDetachChild();
			void testGetDescendants();

			Node *rootNode = nullptr,
				 *parentA = nullptr,
				 *childA0 = nullptr,
				 *childA1 = nullptr,
				 *childA2 = nullptr,
				 *parentB = nullptr,
				 *childB0 = nullptr,
				 *childB1 = nullptr,
				 *childB2 = nullptr,
				 *parentC = nullptr,
				 *childC0 = nullptr,
				 *childC1 = nullptr,
				 *childC2 = nullptr,
				 *parentD = nullptr,
				 *childD0 = nullptr,
				 *childD1 = nullptr,
				 *childD2 = nullptr,
				 *nodeA = nullptr,
				 *nodeB = nullptr,
				 *nodeC = nullptr,
				 *nodeD = nullptr;
	};
}

#endif
