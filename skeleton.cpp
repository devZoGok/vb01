#include "skeleton.h"
#include "animation.h"
#include "animationController.h"
#include "box.h"
#include "ikSolver.h"

using namespace std;

namespace vb01{
	Skeleton::Skeleton(string name){
		this->name = name;
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
		Bone *rootBone = getRootBone();
		Bone *ikTarget = ikBone->getIkTarget();
		Bone *boneChain[chainLength];
		Bone *ikBoneAncestor = ikBone;
		Vector3 boneIkPos[chainLength];
	   	Vector3 targetPos = rootBone->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));

		for(int i = 0; i < chainLength; i++){
			boneChain[i] = ikBoneAncestor;
			ikBoneAncestor = (Bone*)ikBoneAncestor->getParent();
			boneIkPos[i] = rootBone->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3::VEC_ZERO));
			boneIkPos[i] = boneChain[i]->getModelSpacePos();
		}

		IkSolver::calculateFabrik(chainLength, boneChain, boneIkPos, targetPos);

		transformIkChain(chainLength, boneChain, boneIkPos, targetPos);
	}

	void Skeleton::transformIkChain(int chainLength, Bone *boneChain[], Vector3 boneIkPos[], Vector3 targetPos){
		Node *rootBone = getRootBone()->getParent();
		float boneAngles[chainLength];
		Vector3 axis[chainLength];
		for(int i = chainLength - 1; i >= 0; i--){
			boneChain[i]->setOrientation(boneChain[i]->getRestRot());
			Vector3 dir = ((i == 0 ? targetPos : boneIkPos[i - 1]) - boneIkPos[i]).norm();
			Vector3 boneAxis = 
					(rootBone->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3::VEC_J)) - 
					rootBone->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3::VEC_ZERO)))
					.norm();

			Vector3 rotAxis = boneAxis.cross(dir).norm();
			float angle = boneAxis.getAngleBetween(dir);

			if(rotAxis == Vector3::VEC_ZERO){
				angle = 0;
				rotAxis = Vector3::VEC_I;
			}

			boneAngles[i] = angle; 
			axis[i] = rotAxis;

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
