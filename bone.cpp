#include"bone.h"
#include"skeleton.h"

using namespace std;

namespace vb01{
	Bone::Bone(string name, float length, Vector3 pos, Quaternion rot, Vector3 scale) : Node(pos, rot, scale, name){
		this->name = name;
		this->length = length;
	}

	void Bone::lookAt(Vector3 newDir, Vector3 newUp){
		Node::lookAt(newDir, newUp);
		for(int i = 0; i < 3; i++)
			this->initAxis[i] = globalAxis[i];
		restPos = pos;
		restRot = orientation;
		restScale = scale;
	}

	Vector3 Bone::getModelSpacePos(){
		/*
		Vector3 modelSpacePos = Vector3::VEC_ZERO;
		Bone *rootBone = skeleton->getRootBone();
		vector<Node*> boneHierarchy = getAncestors(rootBone);

		while(!boneHierarchy.empty()){
			int id = boneHierarchy.size() - 1;
			Bone *curBone = ((Bone*)boneHierarchy[id]);
			Bone *par = (Bone*)curBone->getParent();

			Vector3 axis[]{ 
				curBone != rootBone? par->getInitAxis(0) : Vector3::VEC_I,
				curBone != rootBone? par->getInitAxis(1) : Vector3::VEC_J,
				curBone != rootBone? par->getInitAxis(2) : Vector3::VEC_K
			};

			Vector3 rPos = curBone->getRestPos();
			modelSpacePos = modelSpacePos + axis[0] * rPos.x + axis[1] * rPos.y + axis[2] * rPos.z;
			boneHierarchy.pop_back();
		}
		*/
		Node *modelNode = skeleton->getRootBone()->getParent();
		Vector3 modelSpacePos = modelNode->globalToLocalPosition(localToGlobalPosition(Vector3::VEC_ZERO));

		return modelSpacePos;
	}

	void Bone::setPosePos(Vector3 p){
		this->posePos = p;

		setPosition(restPos);
		Vector3 parentSpacePosePos = parent->globalToLocalPosition(localToGlobalPosition(p));
		setPosition(parentSpacePosePos);
	}

	void Bone::setPoseRot(Quaternion r){
		this->poseRot = r;

		Vector3 rotAxis = r.getAxis();
		rotAxis =
		   	parent->globalToLocalPosition(localToGlobalPosition(rotAxis)) - 
		   	parent->globalToLocalPosition(localToGlobalPosition(Vector3::VEC_ZERO)); 

		Quaternion parentSpacePoseRot = Quaternion(r.getAngle(), rotAxis);

		setOrientation(parentSpacePoseRot * restRot);
	}

	void Bone::setPoseScale(Vector3 s){
		this->poseScale = s;
		//setScale(restScale+poseScale);
	}
}
