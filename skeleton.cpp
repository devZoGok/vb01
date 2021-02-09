#include "skeleton.h"
#include "animation.h"
#include "animationController.h"
#include "box.h"
#include "ikSolver.h"
#include <glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace std;
using namespace glm;

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
		Bone *ikTarget = ikBone->getIkTarget();
		Bone *subBase = (Bone*)ikTarget->getParent();

		const int chainLength = ikBone->getIkChainLength();
		Vector3 boneIkPos[chainLength];
		Bone *boneChain[chainLength];
		Bone *ikBoneAncestor = ikBone;

		for(int i = 0; i < chainLength; i++){
			boneChain[i] = ikBoneAncestor;
			ikBoneAncestor = (Bone*)ikBoneAncestor->getParent();
			boneChain[i]->setPosePos(Vector3::VEC_ZERO);
			boneChain[i]->setPoseRot(Quaternion::QUAT_W);
			boneChain[i]->setPoseScale(Vector3::VEC_IJK);
		}
		for(int i = 0; i < chainLength; i++)
			boneIkPos[i] = subBase->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3::VEC_ZERO));

	   	Vector3 targetPos = subBase->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));
		IkSolver::calculateFabrik(chainLength, boneChain, boneIkPos, targetPos);
		transformIkChain(chainLength, boneChain, boneIkPos, ikTarget);
	}

	void Skeleton::transformIkChain(int chainLength, Bone *boneChain[], Vector3 boneIkPos[], Bone* ikTarget){
		Bone *subBase = (Bone*)ikTarget->getParent();
		for(int i = chainLength - 1; i >= 0; i--){
			Vector3 tailPosBoneSpace, tailRestPosBoneSpace;
			if(i == 0){
				tailPosBoneSpace = subBase->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));
				tailRestPosBoneSpace = subBase->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3(0, boneChain[i]->getLength(), 0)));
			}
			else{
				tailPosBoneSpace = boneIkPos[i - 1];
				tailRestPosBoneSpace = subBase->globalToLocalPosition(boneChain[i - 1]->localToGlobalPosition(Vector3::VEC_ZERO));
			}
			Vector3 headRestPosBoneSpace = subBase->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3::VEC_ZERO));
			Vector3 headDir = (tailPosBoneSpace - headRestPosBoneSpace).norm();
			Vector3 boneDir = (tailRestPosBoneSpace - headRestPosBoneSpace).norm();

			Vector3 axis = boneDir.cross(headDir).norm();
			axis = 
				boneChain[i]->globalToLocalPosition(subBase->localToGlobalPosition(axis)) - 
				boneChain[i]->globalToLocalPosition(subBase->localToGlobalPosition(Vector3::VEC_ZERO));
			if(axis == Vector3::VEC_ZERO)
				axis = Vector3::VEC_I;

			float angle = boneDir.getAngleBetween(headDir);
			boneChain[i]->setPoseRot(Quaternion(angle, axis));
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
