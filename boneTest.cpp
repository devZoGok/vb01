#include "boneTest.h"
#include "model.h"
#include "root.h"
#include "skeleton.h"
#include "mesh.h"

#include <string>

using namespace std;

namespace vb01{
	BoneTest::BoneTest(){
	}

	void BoneTest::setUp(){
		string PATH="/home/dominykas/c++/v/";

		Root *root = Root::getSingleton();
		rootNode = root->getRootNode();
		model = new Model(PATH + "../vb01/test.vb");
		Material *glMat=new Material();
		glMat->setTexturingEnabled(true);
		glMat->setLightingEnabled(false);
		glMat->addDiffuseMap(PATH+"defaultTexture.jpg");
		//model->setMaterial(glMat);
		rootNode->attachChild(model);
	}

	void BoneTest::tearDown(){
		rootNode->dettachChild(model);
		delete model;
	}

	void BoneTest::testGetModelSpacePos(){
		float eps = .0001;

		int numChildren = model->getNumChildren();
		Skeleton *skeleton = model->getChild(numChildren - 1)->getMesh(0)->getSkeleton();
		Bone *bone = skeleton->getBone("ik");
		Vector3 initModelPos = bone->getModelSpacePos();
		model->setPosition(Vector3(1, 2, 3));
		Vector3 deltaModelPos = bone->getModelSpacePos();

		CPPUNIT_ASSERT(initModelPos.getDistanceFrom(deltaModelPos) <= eps);
	}
}
