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
		Vector3 boneIkPos[chainLength], targetPos = ikTarget->getModelSpacePos();
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
				bool backward = i % 2;
				for(int j = 0; j < chainLength; j++){
					int boneId = (backward ? j : chainLength - 1 - j);
					Vector3 ikPos = (j == 0 ? (backward ? targetPos : startPos) : boneIkPos[boneId]);
					Vector3 fromBoneToIkPos = ikPos - boneChain[boneId]->getModelSpacePos();
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

		Bone *rootBone = getRootBone();
		for(int i = 0; i < chainLength; i++)
			(boneChain[i])->lookAt(boneIkPos[i], rootBone->getParent());
	}

	void Skeleton::addBone(Bone *bone, Bone *parent){
		if(parent)
			parent->attachChild(bone);
		bone->setSkeleton(this);
		bones.push_back(bone);
	}

	Bone* Skeleton::getBone(string name){
		Bone *bone=nullptr;
		for(Bone *b : bones)
			if(b->getName()==name){
				bone=b;
				break;
			}
		return bone;
	}

}
