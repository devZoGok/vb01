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
			boneIkPos[i] = boneChain[i]->getBoneSpaceRestPos(subBase);
		}

	   	Vector3 targetPos = subBase->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));
		IkSolver::calculateFabrik(chainLength, boneChain, boneIkPos, targetPos);
		transformIkChain(chainLength, boneChain, boneIkPos, ikTarget);
	}

	void Skeleton::transformIkChain(int chainLength, Bone *boneChain[], Vector3 boneIkPos[], Bone* ikTarget){
		Bone *subBase = (Bone*)ikTarget->getParent();
		Vector3 rotAxis[chainLength];
		float angles[chainLength];

		for(int i = 0; i < chainLength; i++){
			Vector3 tailPosBoneSpace;
			Vector3 tailRestPosBoneSpace;
			if(i == 0){
				tailPosBoneSpace = subBase->globalToLocalPosition(ikTarget->localToGlobalPosition(Vector3::VEC_ZERO));
				tailRestPosBoneSpace = ikTarget->getBoneSpaceRestPos(subBase);
			}
			else{
				tailPosBoneSpace = boneIkPos[i - 1];
				tailRestPosBoneSpace = boneChain[i - 1]->getBoneSpaceRestPos(subBase);
			}
			Vector3 headRestPosBoneSpace = boneChain[i]->getBoneSpaceRestPos(subBase);
			Vector3 headDir = (tailPosBoneSpace - headRestPosBoneSpace).norm();
			Vector3 boneDir = (tailRestPosBoneSpace - headRestPosBoneSpace).norm();

			float angle = boneDir.getAngleBetween(headDir);
			angles[i] = angle;

			Vector3 axis = boneDir.cross(headDir).norm();
			if(axis == Vector3::VEC_ZERO)
				axis = Vector3::VEC_I;

			axis = (
					subBase->getInitAxis(0) * axis.x +
					subBase->getInitAxis(1) * axis.y +
					subBase->getInitAxis(2) * axis.z 
				).norm();

			mat3 mat;
			mat[0][0] = boneChain[i]->getInitAxis(0).x;
			mat[1][0] = boneChain[i]->getInitAxis(0).y;
			mat[2][0] = boneChain[i]->getInitAxis(0).z;
			mat[0][1] = boneChain[i]->getInitAxis(1).x;
			mat[1][1] = boneChain[i]->getInitAxis(1).y;
			mat[2][1] = boneChain[i]->getInitAxis(1).z;
			mat[0][2] = boneChain[i]->getInitAxis(2).x;
			mat[1][2] = boneChain[i]->getInitAxis(2).y;
			mat[2][2] = boneChain[i]->getInitAxis(2).z;
			vec3 localAxis = vec3(axis.x, axis.y, axis.z) * inverse(mat);

			rotAxis[i] = Vector3(localAxis.x, localAxis.y, localAxis.z).norm();
		}

		for(int i = 0; i < chainLength; i++){
			for(int j = i + 1; j < chainLength; j++)
				angles[i] -= angles[j];

			if(angles[i] < 0)
				angles[i] = 0;
		}

		for(int i = 0; i < chainLength; i++)
			boneChain[i]->setPoseRot(Quaternion(angles[i], rotAxis[i]));
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
