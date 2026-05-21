#ifndef IK_SOLVER_TEST_H
#define IK_SOLVER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "bone.h"

namespace vb01{
	class Skeleton;
	class Node;
	class Bone;

	class IkSolverTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(IkSolverTest);
		CPPUNIT_TEST(testSolveIk);
		CPPUNIT_TEST_SUITE_END();
		public:
			IkSolverTest(){}
			~IkSolverTest(){}
			void setUp();
			void tearDown();
			void testSolveIk();
		private:
			Skeleton *skeleton = nullptr;
			Node *rootNode = nullptr;
			int chainLength = 3;
			Vector3 *ikPos;
			Bone *ikBone = nullptr, **boneChain;

			void resetIkData();
	};
}

#endif
