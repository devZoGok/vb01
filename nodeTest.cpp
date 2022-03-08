#include "nodeTest.h"
#include "node.h"
#include "root.h"

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
	}

	void NodeTest::tearDown(){
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
