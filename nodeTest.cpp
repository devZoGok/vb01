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

		parentA = new Node();
		childA0 = new Node();
		childA1 = new Node();
		childA2 = new Node();
		parentA->attachChild(childA0);
		parentA->attachChild(childA1);
		parentA->attachChild(childA2);
		parentB = new Node();
		childB0 = new Node();
		childB1 = new Node();
		childB2 = new Node();
		parentB->attachChild(childB0);
		parentB->attachChild(childB1);
		parentB->attachChild(childB2);
		parentC = new Node();
		childC0 = new Node();
		childC1 = new Node();
		childC2 = new Node();
		parentC->attachChild(childC0);
		parentC->attachChild(childC1);
		parentC->attachChild(childC2);
		parentD = new Node();
		childD0 = new Node();
		childD1 = new Node();
		childD2 = new Node();
		parentD->attachChild(childD0);
		parentD->attachChild(childD1);
		parentD->attachChild(childD2);
		parentA->attachChild(parentB);
		parentB->attachChild(parentC);
		parentC->attachChild(parentD);

		rootNode->attachChild(nodeA);
		nodeA->attachChild(nodeB);
		/*
		nodeA->attachChild(nodeC);
		nodeA->attachChild(nodeD);
		*/
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

	   	parentA->getDescendants(parentADescendants);
	   	parentB->getDescendants(parentBDescendants);
	   	parentC->getDescendants(parentCDescendants);
	   	parentD->getDescendants(parentDDescendants);

		parentD->getDescendants(parentDDescendants);
		CPPUNIT_ASSERT(parentDDescendants[0] == childD0);
		CPPUNIT_ASSERT(parentDDescendants[1] == childD1);
		CPPUNIT_ASSERT(parentDDescendants[2] == childD2);
	}
}
