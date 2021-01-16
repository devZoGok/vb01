#include "boneTest.h"
#include "root.h"
#include "skeleton.h"

#include <string>

using namespace std;

namespace vb01{
	void BoneTest::setUp(){
		rootNode = Root::getSingleton()->getRootNode();
		modelNodeParent = new Node();
		modelNode = new Node();
		rootNode->attachChild(modelNodeParent);
		modelNodeParent->attachChild(modelNode);

		skeleton = new Skeleton();

		Bone *rootBone = new Bone("rootBone", 1, Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK);
		Bone *pelvis = new Bone("pelvis", 1, Vector3(0, 2, 0), Quaternion::QUAT_W, Vector3::VEC_IJK);
		Bone *upperArm = new Bone("upperArm.R", 1, Vector3(0, 2, 0), Quaternion::QUAT_W, Vector3::VEC_IJK);
		Bone *lowerArm = new Bone("lowerArm.R", 1, Vector3(0, 1, 0), Quaternion::QUAT_W, Vector3::VEC_IJK);

		skeleton->addBone(rootBone, (Bone*)modelNode);
		rootBone->lookAt(Vector3::VEC_K, Vector3::VEC_J);

		skeleton->addBone(pelvis, rootBone);
		pelvis->lookAt(Vector3::VEC_K, Vector3::VEC_J);

		skeleton->addBone(upperArm, pelvis);
		upperArm->lookAt(Vector3::VEC_K, Vector3(-1, 0 ,0));

		skeleton->addBone(lowerArm, upperArm);
		lowerArm->lookAt(Vector3::VEC_K, Vector3(0, 1 ,0));
	}

	void BoneTest::tearDown(){
	}

	void BoneTest::testGetModelSpacePos(){
		float eps = .0001;
		Bone *lowerArm = skeleton->getBone("lowerArm.R");
		Vector3 testModelSpacePos = Vector3(-1, 4, 0);
		Bone *rootBone = skeleton->getRootBone();

		CPPUNIT_ASSERT(lowerArm->getBoneSpaceRestPos(rootBone).getDistanceFrom(testModelSpacePos) <= eps);

		modelNode->setPosition(Vector3::VEC_ZERO);
		modelNode->setOrientation(Quaternion(.5, Vector3(1, 2, 3).norm()));
		CPPUNIT_ASSERT(lowerArm->getBoneSpaceRestPos(rootBone).getDistanceFrom(testModelSpacePos) <= eps);
		
		modelNode->setOrientation(Quaternion::QUAT_W);
		modelNode->setPosition(Vector3(10, 20, 30));
		CPPUNIT_ASSERT(lowerArm->getBoneSpaceRestPos(rootBone).getDistanceFrom(testModelSpacePos) <= eps);

		modelNode->setOrientation(Quaternion(.5, Vector3(1, 2, 3).norm()));
		modelNode->setPosition(Vector3(10, 20, 30));
		CPPUNIT_ASSERT(lowerArm->getBoneSpaceRestPos(rootBone).getDistanceFrom(testModelSpacePos) <= eps);

		modelNodeParent->setOrientation(Quaternion(.3, Vector3(1, 2, 3).norm()));
		modelNodeParent->setPosition(Vector3(100, 200, 300));
		modelNode->setOrientation(Quaternion(.5, Vector3(1, 2, 3).norm()));
		modelNode->setPosition(Vector3(10, 20, 30));
		CPPUNIT_ASSERT(lowerArm->getBoneSpaceRestPos(rootBone).getDistanceFrom(testModelSpacePos) <= eps);
	}
}
