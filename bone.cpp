#include"bone.h"

using namespace std;

namespace vb01{
	Bone::Bone(string name, Vector3 axis[3], Vector3 pos,Quaternion rot, Vector3 scale) : Node(pos,rot,scale){
		this->name=name;
		this->restPos=pos;
		this->restRot=rot;
		this->restScale=scale;
		for(int i=0;i<3;i++)
			this->initAxis[i]=axis[i];
		//lookAt(axis[1],axis[2],parent);
	}

	void Bone::setPosePos(Vector3 p){
		this->posePos=p;
		setPosition(restPos+posePos);
	}

	void Bone::setPoseRot(Quaternion r){
		this->poseRot=r;
		setOrientation(poseRot*restRot);
	}

	void Bone::setPoseScale(Vector3 s){
		this->poseScale=s;
		setScale(restScale+poseScale);
	}
}
