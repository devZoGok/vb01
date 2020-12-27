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
		nodeA = new Node();
		nodeB = new Node();
		nodeC = new Node();
		nodeD = new Node();

		firstChainNode = new Node();
		rootNode->attachChild(firstChainNode);
		secondChainNode = new Node();
		firstChainNode->attachChild(secondChainNode);
		thirdChainNode = new Node();
		secondChainNode->attachChild(thirdChainNode);
	}

	void NodeTest::tearDown(){
		/*
		delete nodeD;
		delete nodeC;
		delete nodeB;
		nodeA->dettachChild(nodeB);
		delete nodeA;
		*/
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

	void NodeTest::testGlobalToLocalOrientation(){
	}

	void NodeTest::testLocalToGlobalOrientation(){
		firstChainNode->setOrientation(Quaternion::QUAT_W);
		secondChainNode->setOrientation(Quaternion::QUAT_W);
		thirdChainNode->setOrientation(Quaternion::QUAT_W);

		CPPUNIT_ASSERT(firstChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(secondChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == Quaternion::QUAT_W);

		Quaternion q0 = Quaternion(.2, Vector3(0, 1, 0));
		thirdChainNode->setOrientation(q0);
		CPPUNIT_ASSERT(thirdChainNode->localToGlobalOrientation(Quaternion::QUAT_W) == q0);
	}
	void NodeTest::testDetachChild(){
		nodeA->dettachChild(nodeB);
		CPPUNIT_ASSERT(nodeA->getChild(0) != nodeB);
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
			nodeA->lookAt(dir[i], rootNode);
			CPPUNIT_ASSERT(nodeA->getGlobalAxis(2).getAngleBetween(dir[i]) < maxAngle);
		}
	}

	void NodeTest::testAdjustUp(){
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
			/*
			nodeA->lookAt(dir[i], rootNode);
			CPPUNIT_ASSERT(nodeA->getGlobalAxis(2).getAngleBetween(dir[i]) < maxAngle);
			*/
		}
	}

	void NodeTest::testGetDescendants(){
		vector<Node*> parentADescendants;
		vector<Node*> parentBDescendants;
		vector<Node*> parentCDescendants;
		vector<Node*> parentDDescendants;

		/*
	   	parentA->getDescendants(parentADescendants);
	   	parentB->getDescendants(parentBDescendants);
	   	parentC->getDescendants(parentCDescendants);
	   	parentD->getDescendants(parentDDescendants);

		parentD->getDescendants(parentDDescendants);
		CPPUNIT_ASSERT(parentDDescendants[0] == childD0);
		CPPUNIT_ASSERT(parentDDescendants[1] == childD1);
		CPPUNIT_ASSERT(parentDDescendants[2] == childD2);
		*/
	}
}
