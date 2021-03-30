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
	Skeleton::Skeleton(AnimationController *controller, string name){
		this->name = name;
		this->controller = controller;
	}

	void Skeleton::update(){
		updateIk();
		controller->update();
	}

	void Skeleton::updateIk(){
		for(Bone *b : bones){
			if(b->getIkTarget()){
				const int chainLength = b->getIkChainLength();
				Bone **boneChain = getIkBoneChain(b);
				for(int i = 0; i < chainLength; i++){
					boneChain[i]->setPosePos(Vector3::VEC_ZERO);
					boneChain[i]->setPoseRot(Quaternion::QUAT_W);
					boneChain[i]->setPoseScale(Vector3::VEC_IJK);
				}
				delete[] boneChain;
			}
		}
		for(Bone *b : bones)
			if(b->getIkTarget())
				solveIk(b);
	}

	void Skeleton::solveIk(Bone *ikBone){
		const int chainLength = ikBone->getIkChainLength();
		Bone *ikTarget = ikBone->getIkTarget();
		Bone *subBase = (Bone*)ikTarget->getParent();

		Bone **boneChain = getIkBoneChain(ikBone);
		Vector3 *boneIkPos = new Vector3[chainLength];
		for(int i = chainLength - 1; i >= 0; i--)
			boneIkPos[i] = subBase->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3::VEC_ZERO));

	   	Vector3 targetPos = subBase->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));
		IkSolver::calculateFabrik(chainLength, boneChain, boneIkPos, targetPos);
		transformIkChain(chainLength, boneChain, boneIkPos, ikTarget);

		delete[] boneIkPos;
		delete[] boneChain;
	}

	void Skeleton::transformIkChain(int chainLength, Bone **boneChain, Vector3 boneIkPos[], Bone* ikTarget){
		Bone *subBase = (Bone*)ikTarget->getParent();
		for(int i = chainLength - 1; i >= 0; i--){
			boneChain[i]->setPoseRot(Quaternion::QUAT_W);

			Vector3 tailPosBoneSpace;
			if(i == 0)
				tailPosBoneSpace = subBase->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));
			else
				tailPosBoneSpace = boneIkPos[i - 1];

			Vector3 tailRestPosBoneSpace = subBase->globalToLocalPosition(boneChain[i]->localToGlobalPosition(Vector3(0, boneChain[i]->getLength(), 0)));
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

	Bone** Skeleton::getIkBoneChain(Bone *ikBone){
		int chainLength = ikBone->getIkChainLength();
		Bone **chain = new Bone*[chainLength];
		Bone *ikBoneAncestor = ikBone;
		for(int i = 0; i < chainLength; i++){
			chain[i] = ikBoneAncestor;
			ikBoneAncestor = (Bone*)ikBoneAncestor->getParent();
		}
		return chain;
	}
}
