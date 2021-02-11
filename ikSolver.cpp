#include "ikSolver.h"
#include "bone.h"

namespace vb01{
	void IkSolver::calculateFabrik(int chainLength, Bone **boneChain, Vector3 boneIkPos[], Vector3 targetPos){
		float sumLengths = 0;
		for(int i = 0; i < chainLength; i++)
			sumLengths += boneChain[i]->getLength();
		
		Bone *subBase = (Bone*)boneChain[0]->getIkTarget()->getParent();
		Vector3 startPos = subBase->globalToLocalPosition(boneChain[chainLength - 1]->localToGlobalPosition(Vector3::VEC_ZERO));
		if(startPos.getDistanceFrom(targetPos) < sumLengths){
			int numIterations = 500;
	
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
					}
					bonePos = boneIkPos[boneId];
					fromBoneToIkPos = (ikPos - bonePos).norm();
					boneIkPos[boneId] = ikPos - fromBoneToIkPos * length;
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
	}
}
