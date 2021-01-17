#include "ikSolverTest.h"
#include "ikSolver.h"
#include "skeleton.h"

namespace vb01{
	void IkSolverTest::setUp(){
		rootNode = Root::getSingleton()->getRootNode();

		skeleton = new Skeleton("");

		Bone *boneA = new Bone("A", 1, Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK);
		Bone *boneB = new Bone("B", 1, Vector3::VEC_J, Quaternion::QUAT_W, Vector3::VEC_IJK);
		Bone *boneC = new Bone("C", 1, Vector3::VEC_J, Quaternion::QUAT_W, Vector3::VEC_IJK);
		Bone *boneD = new Bone("D", 1, Vector3::VEC_J, Quaternion::QUAT_W, Vector3::VEC_IJK);
		ikBone = new Bone("ik", 1, Vector3(0, 4, 0), Quaternion::QUAT_W, Vector3::VEC_IJK);

		skeleton->addBone(boneA, (Bone*)rootNode);
		skeleton->addBone(boneB, boneA);
		skeleton->addBone(boneC, boneB);
		skeleton->addBone(boneD, boneC);
		skeleton->addBone(ikBone, boneA);

		boneA->lookAt(Vector3::VEC_K, Vector3::VEC_J);
		boneB->lookAt(Vector3::VEC_K, Vector3::VEC_J);
		boneC->lookAt(Vector3::VEC_K, Vector3::VEC_J);
		boneD->lookAt(Vector3::VEC_K, Vector3::VEC_J);

		boneD->setIkChainLength(chainLength);
		boneD->setIkTarget(ikBone);

		ikPos = new Vector3[chainLength];
		boneChain = new Bone*[chainLength];

		resetIkData();
	}

	void IkSolverTest::tearDown(){
	}

	void IkSolverTest::testSolveIk(){
		Vector3 ikBonePos = Vector3(1, 1, 1) * 5 + Vector3(0, 1, 0);
		resetIkData();
		IkSolver::calculateFabrik(chainLength, boneChain, ikPos, ikBonePos);

		float eps = .01;

		for(int i = chainLength - 1; i >= 0; i--)
			CPPUNIT_ASSERT(ikPos[i].getDistanceFrom(Vector3(0, 1, 0) + Vector3(1, 1, 1).norm() * (chainLength - 1 - i)) <= eps);

		ikBonePos = Vector3(0, 1, 0) * 2 + Vector3(2, 0, 0);
		resetIkData();
		IkSolver::calculateFabrik(chainLength, boneChain, ikPos, ikBonePos);

		/*
		CPPUNIT_ASSERT(ikPos[1].getDistanceFrom(Vector3(0.171451, 1.98516, 0)) <= eps);
		CPPUNIT_ASSERT(ikPos[0].getDistanceFrom(Vector3(1.08232, 2.39756, 0)) <= eps);
		*/

	}

	void IkSolverTest::resetIkData(){
		Bone *rootBone = skeleton->getRootBone();
		Bone *ikBoneAncestor = skeleton->getBone("D");

		for(int i = 0; i < chainLength; i++){
			boneChain[i] = ikBoneAncestor;
			ikBoneAncestor = (Bone*)ikBoneAncestor->getParent();
			ikPos[i] = boneChain[i]->getModelSpacePos();
		}
	}
}
