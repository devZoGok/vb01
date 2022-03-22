#ifndef NODE_TEST_H
#define NODE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Node;

	class NodeTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(NodeTest);
		CPPUNIT_TEST(testClonedMeshes);
		CPPUNIT_TEST(testClonedDrivers);
		CPPUNIT_TEST(testClonedSkeletons);
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
			void setupOriginalNodes();
			void testClonedMeshes();
			void testClonedDrivers();
			void testClonedSkeletons();
			void testAdjustDir();
			void testAdjustUp();
			void testDetachChild();
			void testGetAncestors();
			void testLocalToGlobalPosition();
			void testGlobalToLocalPosition();
			void testLocalToGlobalOrientation();
			void testGlobalToLocalOrientation();

			Node *rootNode = nullptr;
			Node *ancestorA = nullptr;
			Node *ancestorB = nullptr;
			Node *ancestorC = nullptr;
			Node *ancestorD = nullptr;
			Node *lookNodeParent = nullptr;
			Node *lookNode = nullptr;
			Node *firstChainNode = nullptr;
			Node *secondChainNode = nullptr;
			Node *thirdChainNode = nullptr;
			Node *originalA = nullptr;
			Node *originalB = nullptr;
			Node *originalC = nullptr;
			Node *originalD = nullptr;
	};
}

#endif
