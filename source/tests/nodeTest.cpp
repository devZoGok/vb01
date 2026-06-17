#include "nodeTest.h"
#include "bone.h"
#include "root.h"
#include "skeleton.h"
#include "mesh.h"
#include "meshData.h"
#include "keyframeChannel.h"

#include <vector>

using namespace std;

namespace vb01{
	NodeTest::NodeTest(){
	}

	void NodeTest::setUp(){
		rootNode = Root::getSingleton()->getRootNode();

		lookNodeParent = new Node();
		rootNode->attachChild(lookNodeParent);
		lookNode = new Node();
		lookNodeParent->attachChild(lookNode);

		firstChainNode = new Node();
		rootNode->attachChild(firstChainNode);
		secondChainNode = new Node();
		firstChainNode->attachChild(secondChainNode);
		thirdChainNode = new Node();
		secondChainNode->attachChild(thirdChainNode);

		ancestorA = new Node();
		ancestorB = new Node();
		ancestorC = new Node();
		ancestorD = new Node();
	
		rootNode->attachChild(ancestorA);
		ancestorA->attachChild(ancestorB);
		ancestorB->attachChild(ancestorC);
		ancestorC->attachChild(ancestorD);

		originalA = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "A");
		originalB = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "B");
		originalC = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "C");
		originalD = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "D");

		rootNode->attachChild(originalA);
		originalA->attachChild(originalB);
		originalA->attachChild(originalC);
		originalC->attachChild(originalD);
	}

	void NodeTest::tearDown(){
	}

	void NodeTest::setupOriginalNodes(){

	}

	void NodeTest::testClonedMeshes(){
			int numTris = 3;
			u32 *indices =  new u32[3 * 3];
			originalD->attachMesh(new Mesh(MeshData(nullptr, indices, numTris)));

			Node *clonedA = originalA->clone();
			vector<Node*> clonedDescendands;
			clonedA->getDescendants(clonedDescendands);
			Node *clonedD = nullptr;

			for(Node *desc : clonedDescendands)
					if(desc->getName() == "D"){
							clonedD = desc;
							break;
					}

			CPPUNIT_ASSERT(clonedD->getMesh(0)->getMeshBase().vertices == nullptr);
			CPPUNIT_ASSERT(clonedD->getMesh(0)->getMeshBase().indices == indices);
			CPPUNIT_ASSERT(clonedD->getMesh(0)->getMeshBase().numTris == numTris);
	}

	void NodeTest::testClonedDrivers(){
			vector<Keyframe> keyframes;
			KeyframeChannelType channelType = KeyframeChannelType::POS_X;
			string animName = "D";
			KeyframeChannel channel = KeyframeChannel::createKeyframeChannel(channelType, animName, keyframes);

			int numKeys = 1;
			Mesh *mesh = new Mesh(MeshData(nullptr, nullptr, 0, "", nullptr, 0, "", new MeshData::ShapeKey[numKeys], numKeys));
			originalD->attachMesh(mesh);

			Driver *driver = new Driver(&mesh->getMeshBase().shapeKeys[0], channel, Driver::POS_X);
			originalB->addDriver(driver);

			Node *clonedA = originalA->clone();
			vector<Node*> clonedDescendands;
			clonedA->getDescendants(clonedDescendands);
			Node *clonedB = nullptr;
			Node *clonedD = nullptr;

			for(Node *desc : clonedDescendands){
					if(desc->getName() == "B")
							clonedB = desc;
					else if(desc->getName() == animName)
							clonedD = desc;
			}

			Driver *driverClone = clonedB->getDriver(0);
			CPPUNIT_ASSERT(driverClone->getAnimatable() == clonedD->getMesh(0)->getShapeKey(0));
			CPPUNIT_ASSERT(driverClone->getKeyframeChannel().type == channelType);
			CPPUNIT_ASSERT(driverClone->getKeyframeChannel().animatable == animName);
			CPPUNIT_ASSERT(driverClone->getType() == Driver::POS_X);
	}

	void NodeTest::testClonedSkeletons(){
			Skeleton *skeleton = new Skeleton();
			originalC->dettachChild(originalD);
			originalA->dettachChild(originalC);
			originalA->dettachChild(originalB);
			originalC = new Bone(originalC->getName(), .25);
			originalD = new Bone(originalD->getName(), .5);
			originalB = new Bone(originalB->getName(), .1);
			originalC->attachChild(originalD);
			originalA->attachChild(originalC);
			originalA->attachChild(originalB);
			
			dynamic_cast<Bone*>(originalB)->setIkChainLength(2);
			dynamic_cast<Bone*>(originalB)->setIkTarget(originalD->getName());

			skeleton->addBone(dynamic_cast<Bone*>(originalB), nullptr);
			skeleton->addBone(dynamic_cast<Bone*>(originalC), nullptr);
			skeleton->addBone(dynamic_cast<Bone*>(originalD), nullptr);
			originalA->addSkeleton(skeleton);

			Node *clonedA = originalA->clone();
			CPPUNIT_ASSERT(clonedA->getSkeleton(0));

			Skeleton *clonedSkeleton = originalA->getSkeleton(0);
			CPPUNIT_ASSERT(clonedSkeleton->getBone(originalB->getName())->getIkChainLength() == dynamic_cast<Bone*>(originalB)->getIkChainLength());
			CPPUNIT_ASSERT(clonedSkeleton->getBone(originalB->getName())->getIkTarget() == dynamic_cast<Bone*>(originalB)->getIkTarget());
			CPPUNIT_ASSERT(clonedSkeleton->getBone(originalC->getName())->getName() == dynamic_cast<Bone*>(originalC)->getName());
			CPPUNIT_ASSERT(clonedSkeleton->getBone(originalC->getName())->getLength() == dynamic_cast<Bone*>(originalC)->getLength());
			CPPUNIT_ASSERT(clonedSkeleton->getBone(originalD->getName())->getLength() == dynamic_cast<Bone*>(originalD)->getLength());
			CPPUNIT_ASSERT(clonedSkeleton->getBone(originalD->getName())->getName() == dynamic_cast<Bone*>(originalD)->getName());
	}

	void NodeTest::testClonedMeshSkeleton(){
			string name = "skeleton";
			Mesh *mesh = new Mesh(MeshData(nullptr, nullptr, 0, "", nullptr, 0, name));
			Skeleton *skeleton = new Skeleton(name);

			originalD->attachMesh(mesh);
			originalD->addSkeleton(skeleton);

			Node *clonedA = originalA->clone();
			Node *clonedD = nullptr;
			vector<Node*> clonedDescendands;
			clonedA->getDescendants(clonedDescendands);

			for(Node *desc : clonedDescendands)
					if(desc->getName() == "D"){
							clonedD = desc;
							break;
					}

			CPPUNIT_ASSERT(clonedD->getMesh(0)->getSkeleton()->getAttachableName() == name);
	}

	void NodeTest::testGetAncestors(){
		vector<Node*> ancestors = ancestorD->getAncestors();
		CPPUNIT_ASSERT(ancestors[ancestors.size() - 1] == rootNode);

		ancestors = ancestorD->getAncestors(ancestorB);
		CPPUNIT_ASSERT(ancestors[ancestors.size() - 1] == ancestorB);
	}

	void NodeTest::testLocalToGlobalPosition(){
		firstChainNode->setPosition(Vector3::VEC_ZERO);
		secondChainNode->setPosition(Vector3::VEC_ZERO);
		thirdChainNode->setPosition(Vector3::VEC_ZERO);

		CPPUNIT_ASSERT(firstChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == Vector3::VEC_ZERO);

		Vector3 pos0 = Vector3(1, 1, 1);
		thirdChainNode->setPosition(pos0);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos0);

		Vector3 pos1 = Vector3(1, 2, 3);
		secondChainNode->setPosition(pos1);
		thirdChainNode->setPosition(Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos1);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos1);

		Vector3 pos2 = Vector3(1, 2, 1);
		Vector3 pos22 = Vector3(1, 2, 2);
		secondChainNode->setPosition(pos2);
		thirdChainNode->setPosition(pos22);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos2);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalPosition(pos22) == pos2 + pos22);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos2 +  pos22);

		Vector3 pos3 = Vector3(1, 2, 1);
		firstChainNode->setPosition(pos3);
		secondChainNode->setPosition(Vector3::VEC_ZERO);
		thirdChainNode->setPosition(Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(firstChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos3);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos3);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalPosition(Vector3::VEC_ZERO) == pos3);
	}

	void NodeTest::testGlobalToLocalPosition(){
		firstChainNode->setPosition(Vector3::VEC_ZERO);
		secondChainNode->setPosition(Vector3::VEC_ZERO);
		thirdChainNode->setPosition(Vector3::VEC_ZERO);

		CPPUNIT_ASSERT(firstChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(secondChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(thirdChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == Vector3::VEC_ZERO);

		Vector3 pos0 = Vector3(1, 1, 1);
		thirdChainNode->setPosition(pos0);
		CPPUNIT_ASSERT(thirdChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos0);

		Vector3 pos1 = Vector3(1, 2, 3);
		secondChainNode->setPosition(pos1);
		thirdChainNode->setPosition(Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(secondChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos1);
		CPPUNIT_ASSERT(thirdChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos1);

		Vector3 pos2 = Vector3(1, 2, 3);
		secondChainNode->setPosition(pos2);
		thirdChainNode->setPosition(pos2);
		CPPUNIT_ASSERT(secondChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos2);
		CPPUNIT_ASSERT(thirdChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos2 * 2);

		Vector3 pos3 = Vector3(1, 2, 3);
		firstChainNode->setPosition(pos3);
		secondChainNode->setPosition(Vector3::VEC_ZERO);
		thirdChainNode->setPosition(Vector3::VEC_ZERO);
		CPPUNIT_ASSERT(firstChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos3);
		CPPUNIT_ASSERT(secondChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos3);
		CPPUNIT_ASSERT(thirdChainNode->globalToLocalPosition(Vector3::VEC_ZERO) == -pos3);
	}

	void NodeTest::testLocalToGlobalOrientation(){
		firstChainNode->setOrientation(Quaternion::QUAT_W);
		secondChainNode->setOrientation(Quaternion::QUAT_W);
		thirdChainNode->setOrientation(Quaternion::QUAT_W);

		CPPUNIT_ASSERT(firstChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);

		float eps = .001;

		float an = .2;
		Vector3 ax = Vector3(0, 1, 0).norm();

		Quaternion q0 = Quaternion(an, ax);
		secondChainNode->setOrientation(q0);
		thirdChainNode->setOrientation(Quaternion::QUAT_W);
		Quaternion ltg0 = secondChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		Quaternion ltg01 = thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg0.getAxis()) < eps && fabs(an - ltg0.getAngle()) < eps);
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg01.getAxis()) < eps && fabs(an - ltg01.getAngle()) < eps);

		Quaternion q1 = Quaternion(an, ax);
		secondChainNode->setOrientation(Quaternion::QUAT_W);
		thirdChainNode->setOrientation(q1);
		Quaternion ltg1 = secondChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		Quaternion ltg11 = thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		CPPUNIT_ASSERT(ltg1 == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg11.getAxis()) < eps && fabs(an - ltg11.getAngle()) < eps);

		an = .3;
		ax = Vector3(1, 0, 1).norm();
		Quaternion q2 = Quaternion(an, ax);
		Quaternion q21 = Quaternion(an, ax);
		secondChainNode->setOrientation(q2);
		thirdChainNode->setOrientation(q21);
		Quaternion ltg2 = secondChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		Quaternion ltg21 = thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		Quaternion refQuat = q21 * q2;
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg2.getAxis()) < eps && fabs(an - ltg2.getAngle()) < eps);
		CPPUNIT_ASSERT(refQuat.getAxis().getAngleBetween(ltg21.getAxis()) < eps && fabs(refQuat.getAngle() - ltg21.getAngle()) < eps);

		Quaternion q3 = Quaternion(an, ax);
		firstChainNode->setOrientation(q3);
		secondChainNode->setOrientation(Quaternion::QUAT_W);
		thirdChainNode->setOrientation(Quaternion::QUAT_W);
		Quaternion ltg4 = firstChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		Quaternion ltg41 = secondChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		Quaternion ltg42 = thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W);
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg4.getAxis()) < eps && fabs(an - ltg4.getAngle()) < eps);
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg41.getAxis()) < eps && fabs(an - ltg41.getAngle()) < eps);
		CPPUNIT_ASSERT(ax.getAngleBetween(ltg42.getAxis()) < eps && fabs(an - ltg42.getAngle()) < eps);
	}

	void NodeTest::testGlobalToLocalOrientation(){
		firstChainNode->setOrientation(Quaternion::QUAT_W);
		secondChainNode->setOrientation(Quaternion::QUAT_W);
		thirdChainNode->setOrientation(Quaternion::QUAT_W);

		CPPUNIT_ASSERT(firstChainNode->globalToLocalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(secondChainNode->globalToLocalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(thirdChainNode->globalToLocalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);

		float eps = .001;

		float an = .4;
		Vector3 ax = Vector3(1, 1, 0).norm();

		Quaternion q0 = Quaternion(an, ax);
		thirdChainNode->setOrientation(q0);
		Quaternion gtl0 = thirdChainNode->globalToLocalOrientation(Quaternion::QUAT_W);
		CPPUNIT_ASSERT(ax.getAngleBetween(-gtl0.getAxis()) < eps && fabs(an - gtl0.getAngle()) < eps);

		Quaternion q2 = Quaternion(an, ax);
		Quaternion q21 = Quaternion(an, ax);
		secondChainNode->setOrientation(q2);
		thirdChainNode->setOrientation(q21);
		Quaternion gtl21 = thirdChainNode->globalToLocalOrientation(Quaternion::QUAT_W);
		Quaternion refQuat = q21 * q2;
		CPPUNIT_ASSERT(refQuat.getAxis().getAngleBetween(-gtl21.getAxis()) < eps && fabs(refQuat.getAngle() - gtl21.getAngle()) < eps);

		Quaternion q3 = Quaternion(an, ax);
		firstChainNode->setOrientation(q3);
		secondChainNode->setOrientation(Quaternion::QUAT_W);
		thirdChainNode->setOrientation(Quaternion::QUAT_W);
		Quaternion gtl3 = firstChainNode->globalToLocalOrientation(Quaternion::QUAT_W);
		Quaternion gtl31 = secondChainNode->globalToLocalOrientation(Quaternion::QUAT_W);
		Quaternion gtl32 = thirdChainNode->globalToLocalOrientation(Quaternion::QUAT_W);
		CPPUNIT_ASSERT(ax.getAngleBetween(-gtl3.getAxis()) < eps && fabs(an - gtl3.getAngle()) < eps);
		CPPUNIT_ASSERT(ax.getAngleBetween(-gtl31.getAxis()) < eps && fabs(an - gtl31.getAngle()) < eps);
		CPPUNIT_ASSERT(ax.getAngleBetween(-gtl32.getAxis()) < eps && fabs(an - gtl32.getAngle()) < eps);
	}

	void NodeTest::testAdjustDir(){
		float maxAngle = .001;

		Vector3 dir[]{
			Vector3(0, 1, 0),
			Vector3(.8, .2, 0).norm(),
			Vector3(-.6, 0, .9).norm(),
			Vector3(0, -1, -.4).norm(),
			Vector3(1, 1, 1).norm()
		};

		int numDirs = sizeof(dir) / sizeof(Vector3);

		for(int i = 0; i < numDirs; i++){
			lookNode->lookAt(dir[i]);
			CPPUNIT_ASSERT(lookNode->getGlobalAxis(2).getAngleBetween(dir[i]) < maxAngle);
		}

		Quaternion rotQuat = Quaternion(.707, Vector3(1, 0 ,0));
		lookNodeParent->setOrientation(rotQuat);

		for(int i = 0; i < numDirs; i++){
			lookNode->lookAt(dir[i]);
			CPPUNIT_ASSERT(lookNode->getGlobalAxis(2).getAngleBetween(rotQuat * dir[i]) < maxAngle);
		}

		for(int i = 0; i < numDirs; i++){
			lookNodeParent->dettachChild(lookNode);
			lookNode->lookAt(dir[i]);
			lookNodeParent->attachChild(lookNode);
			CPPUNIT_ASSERT(lookNode->getGlobalAxis(2).getAngleBetween(rotQuat * dir[i]) < maxAngle);
		}
	}

	void NodeTest::testAdjustUp(){
		float maxAngle = .001;

		Vector3 up[]{
			Vector3(1, 0, 0),
			Vector3(.8, .2, 0).norm(),
			Vector3(-.6, 0, .9).norm(),
			Vector3(0, -1, -.4).norm(),
			Vector3(1, 1, 1).norm()
		};

		int numDirs = sizeof(up) / sizeof(Vector3);

		lookNodeParent->setOrientation(Quaternion::QUAT_W);

		for(int i = 0; i < numDirs; i++){
			lookNode->lookAt(Vector3::VEC_K, up[i]);
			Vector3 upClamped = Vector3(up[i].x, up[i].y, 0).norm();
			CPPUNIT_ASSERT(lookNode->getGlobalAxis(1).getAngleBetween(upClamped) < maxAngle);
		}

		lookNode->setOrientation(Quaternion::QUAT_W);
		lookNode->lookAt(Vector3::VEC_K, Vector3::VEC_K);
		CPPUNIT_ASSERT(lookNode->getGlobalAxis(1) == Vector3::VEC_J);

		Quaternion rotQuat = Quaternion(.707, Vector3(1, 0 ,0));
		lookNodeParent->setOrientation(rotQuat);

		for(int i = 0; i < numDirs; i++){
			lookNode->lookAt(Vector3::VEC_K, up[i]);
			Vector3 upClamped = Vector3(up[i].x, up[i].y, 0).norm();
			CPPUNIT_ASSERT(lookNode->getGlobalAxis(1).getAngleBetween(rotQuat * upClamped) < maxAngle);
		}

		for(int i = 0; i < numDirs; i++){
				lookNodeParent->dettachChild(lookNode);
			lookNode->lookAt(Vector3::VEC_K, up[i]);
				lookNodeParent->attachChild(lookNode);
			Vector3 upClamped = Vector3(up[i].x, up[i].y, 0).norm();
			CPPUNIT_ASSERT(lookNode->getGlobalAxis(1).getAngleBetween(rotQuat * upClamped) < maxAngle);
		}
	}
}
