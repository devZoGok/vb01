#include "bone.h"
#include "skeleton.h"

using namespace std;

namespace vb01{
	Bone::Bone(string name, float length, Vector3 pos, Quaternion rot, Vector3 scale) : Node(pos, rot, scale, name){
		this->name = name;
		this->length = length;
	}

	void Bone::lookAt(Vector3 newDir, Vector3 newUp){
		Node::lookAt(newDir, newUp);
		updateBoneInfo();
	}

	void Bone::updateBoneInfo(){
		for(int i = 0; i < 3; i++)
			this->initAxis[i] = globalAxis[i];
		restPos = pos;
		restRot = orientation;
		restScale = scale;
	}

	Vector3 Bone::getBoneSpaceRestPos(Bone *bone){
		Bone *rootBone = skeleton->getRootBone();
		Vector3 modelSpacePos = Vector3::VEC_ZERO;
		vector<Node*> boneHierarchy = getAncestors(bone);

		while(!boneHierarchy.empty()){
			int id = boneHierarchy.size() - 1;
			Bone *curBone = ((Bone*)boneHierarchy[id]);
			Bone *par = (Bone*)curBone->getParent();

			Vector3 axis[]{ 
				curBone != rootBone? par->getInitAxis(0) : Vector3::VEC_I,
				curBone != rootBone? par->getInitAxis(1) : Vector3::VEC_J,
				curBone != rootBone? par->getInitAxis(2) : Vector3::VEC_K
			};

			Vector3 rPos = (curBone == bone ? Vector3::VEC_ZERO : curBone->getRestPos());
			modelSpacePos = modelSpacePos + axis[0] * rPos.x + axis[1] * rPos.y + axis[2] * rPos.z;
			boneHierarchy.pop_back();
		}

		return modelSpacePos;
	}

	Vector3 Bone::getModelSpacePos(){
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

	void Bone::animate(float value, KeyframeChannel keyframeChannel){
		Vector3 newPos = getPosePos(), newScale = getPoseScale();
		Quaternion newRot = getPoseRot();
		switch(keyframeChannel.type){
			case KeyframeChannel::POS_X:
				newPos.x = value;
				break;
			case KeyframeChannel::POS_Y:
				newPos.y = value;
				break;
			case KeyframeChannel::POS_Z:
				newPos.z = value;
				break;
			case KeyframeChannel::ROT_W:
				newRot.w = value;
				break;
			case KeyframeChannel::ROT_X:
				newRot.x = value;
				break;
			case KeyframeChannel::ROT_Y:
				newRot.y = value;
				break;
			case KeyframeChannel::ROT_Z:
				newRot.z = value;
				break;
			case KeyframeChannel::SCALE_X:
				newScale.x = value;
				break;
			case KeyframeChannel::SCALE_Y:
				newScale.y = value;
				break;
			case KeyframeChannel::SCALE_Z:
				newScale.z = value;
				break;
		}
		setPosePos(newPos);
		setPoseRot(newRot);
		setPoseScale(newScale);
	}

	float Bone::getDriverValue(Driver::VariableType type){
		float driverValue;
		switch(type){
			case Driver::POS_X:
				driverValue = posePos.x;
				break;
			case Driver::POS_Y:
				driverValue = posePos.y;
				break;
			case Driver::POS_Z:
				driverValue = posePos.z;
				break;
			case Driver::ROT_W:
				driverValue = poseRot.w;
				break;
			case Driver::ROT_X:
				driverValue = poseRot.x;
				break;
			case Driver::ROT_Y:
				driverValue = poseRot.y;
				break;
			case Driver::ROT_Z:
				driverValue = poseRot.z;
				break;
			case Driver::SCALE_X:
				driverValue = poseScale.x;
				break;
			case Driver::SCALE_Y:
				driverValue = poseScale.y;
				break;
			case Driver::SCALE_Z:
				driverValue = poseScale.z;
				break;
		}
		return driverValue;
	}
}
