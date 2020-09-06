#include"skeleton.h"
#include"animation.h"
#include"animationController.h"
#include"box.h"

using namespace std;

namespace vb01{
	Skeleton::Skeleton(string name){
		this->name=name;
		controller = new AnimationController(this);
	}

	void Skeleton::update(){
		for(Bone *b : bones){
			//b->update();
			if(b->getIkTarget())
				solveIk(b);
		}
		controller->update();
	}

	void Skeleton::solveIk(Bone *ikBone){
		const int chainLength = ikBone->getIkChainLength();
		Bone *ikTarget = ikBone->getIkTarget();
		Bone *boneChain[chainLength];
		Bone *ikBoneAncestor = ikBone;
		Vector3 boneIkPos[chainLength];
	   	Vector3 targetPos = ikTarget->getPosition();

		float sumLengths = 0;

		for(int i = 0; i < chainLength; i++){
			sumLengths += ikBoneAncestor->getLength();
			boneChain[i] = ikBoneAncestor;
			ikBoneAncestor = (Bone*)ikBoneAncestor->getParent();
			boneIkPos[i] = boneChain[i]->getModelSpacePos();
		}

		Vector3 startPos = boneChain[chainLength - 1]->getModelSpacePos();
		if(startPos.getDistanceFrom(targetPos) < sumLengths){
			int numIterations = 2;
	
			for(int i = 0 ; i < numIterations; i++){
				bool backward = (i % 2 == 0);
				for(int j = 0; j < chainLength; j++){
					int boneId;
					float length;
					Vector3 ikPos, bonePos, fromBoneToIkPos;
					if(backward){
						boneId = j;
						length = boneChain[boneId]->getLength();
						ikPos = (j == 0 ? targetPos : boneIkPos[boneId - 1]);
						bonePos = boneIkPos[boneId];
						fromBoneToIkPos = (ikPos - bonePos).norm();
						boneIkPos[boneId] = ikPos - fromBoneToIkPos * length;
					}
					else{
						boneId = chainLength - 1 - j;
						if(j == 0){
							ikPos = startPos;
							length = 0;
						}
						else{
							ikPos = boneIkPos[boneId + 1];
							length = boneChain[boneId + 0]->getLength();
						}
						bonePos = boneIkPos[boneId];
						fromBoneToIkPos = (ikPos - bonePos).norm();
						boneIkPos[boneId] = ikPos - fromBoneToIkPos * length;
					}
				}
			}
		}
		else{
			Vector3 startToEndVec = (targetPos - startPos).norm();
			for(int i = chainLength - 1; i >= 0; i--){
				float length;
				Vector3 bonePos;
				if(i == chainLength - 1){
					length = 0;
					bonePos = boneIkPos[i];
				}
				else{
					length = boneChain[i]->getLength();
					bonePos = boneIkPos[i + 1];
				}
				boneIkPos[i] = bonePos + startToEndVec * length;
			}
		}

		float boneAngles[chainLength];
		Vector3 axis[chainLength];
		for(int i = chainLength - 1; i >= 0; i--){
			Vector3 dir = ((i == 0 ? targetPos : boneIkPos[i - 1]) - boneIkPos[i]).norm();
			Vector3 boneAxis = boneChain[i]->getInitAxis(1);
			Vector3 rotAxis = boneAxis.cross(dir).norm();
			float angle = boneAxis.getAngleBetween(dir);

			if(rotAxis == Vector3::VEC_ZERO){
				angle = 0;
				rotAxis = Vector3::VEC_I;
			}

			boneAngles[i] = angle; 
			axis[i] = rotAxis;
		}
		for(int i = 0; i < chainLength; i++){
			for(int j = i + 1; j < chainLength; j++){
				boneAngles[i] -= boneAngles[j];
				if(boneAngles[i] <= 0){
					boneAngles[i] = 0;
					break;
				}
			}
			float angle = boneAngles[i];
			Vector3 rotAxis = axis[i];

			boneChain[i]->setPoseRot(Quaternion(angle, rotAxis));
		}
	}

	void Skeleton::addBone(Bone *bone, Bone *parent){
		if(parent)
			parent->attachChild(bone);
		bone->setSkeleton(this);
		bones.push_back(bone);
	}

	Bone* Skeleton::getBone(string name){
		Bone *bone = nullptr;
		for(Bone *b : bones)
			if(b->getName() == name){
				bone = b;
				break;
			}
		return bone;
	}

}
