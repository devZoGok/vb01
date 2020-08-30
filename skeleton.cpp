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
		Bone *rootBone = getRootBone();
		Node *model = rootBone->getParent();

		const int chainLength = ikBone->getIkChainLength();
		Bone *ikTarget = ikBone->getIkTarget();
		Bone *boneChain[chainLength];
		Bone *ikBoneAncestor = ikBone;
		Vector3 boneIkPos[chainLength];
	   	Vector3 targetPos = ikTarget->getModelSpacePos() + model->globalToLocalPosition(ikTarget->localToGlobalPosition(ikTarget->getPosePos()));

		float sumLengths = 0;

		for(int i = 0; i < chainLength; i++){
			sumLengths += ikBoneAncestor->getLength();
			boneChain[i] = ikBoneAncestor;
			ikBoneAncestor = (Bone*)ikBoneAncestor->getParent();
			boneIkPos[i] = boneChain[i]->getModelSpacePos();
		}

		Vector3 startPos = boneChain[chainLength - 1]->getModelSpacePos();
		if(startPos.getDistanceFrom(targetPos) < sumLengths){
			int numIterations = 500;
	
			for(int i = 0 ; i < numIterations; i++){
				bool backward = (i % 2 == 0);
				for(int j = 0; j < chainLength; j++){
					int boneId = (backward ? j : chainLength - 1 - j);
					Vector3 ikPos = (j == 0 ? (backward ? targetPos : startPos) : boneIkPos[boneId]);
					Vector3 bonePos = boneChain[boneId]->getModelSpacePos();
					Vector3 fromBoneToIkPos = (ikPos - bonePos).norm();
					boneIkPos[boneId] = ikPos - fromBoneToIkPos * boneChain[boneId]->getLength();
				}
			}
		}
		/*
		else{
			Vector3 fromStartToTarget = targetPos - startPos;
			for(int i = 0; i < chainLength; i++){
			}
		}
		*/

		for(int i = chainLength - 1; i >= 0; i--){
			Vector3 dir = ((i == 0 ? targetPos : boneIkPos[i - 1]) - boneChain[i]->getModelSpacePos()).norm() * -1;
			Vector3 boneAxis = boneChain[i]->getInitAxis(1);
			Vector3 rotAxis = dir.cross(boneAxis).norm();
			//rotAxis = (model->localToGlobalPosition(boneChain[i]->globalToLocalPosition(rotAxis))).norm();
			float angle = boneAxis.getAngleBetween(dir);
			if(rotAxis == Vector3::VEC_ZERO){
				angle = 0;
				rotAxis = Vector3::VEC_I;
			}

			boneChain[i]->setPoseRot(Quaternion(angle, rotAxis));
			//((Node*)boneChain[i])->lookAt(dir, model);
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
