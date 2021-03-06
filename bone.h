#ifndef BONE_H
#define BONE_H

#include"node.h"
#include<string>

namespace vb01{
	class Skeleton;

	class Bone : public Node{
		public:
			Bone(std::string, float, Vector3 = Vector3::VEC_ZERO, Quaternion = Quaternion::QUAT_W, Vector3 = Vector3::VEC_ZERO);
			void setPosePos(Vector3 p);
			void setPoseRot(Quaternion r);
			void setPoseScale(Vector3 s);
			void lookAt(Vector3, Vector3, Node*);
			void lookAt(Vector3, Node*);
			Vector3 getModelSpacePos();
			inline Bone* getIkTarget(){return ikTarget;}
			inline void setIkTarget(Bone *target){this->ikTarget = target;}
			inline int getIkChainLength(){return ikChainLength;}
			inline void setIkChainLength(int ikChainLength){this->ikChainLength = ikChainLength;}
			inline float getLength(){return length;}
			inline void setSkeleton(Skeleton *sk){this->skeleton = sk;}
			inline Vector3 getInitAxis(int i){return initAxis[i];}
			inline std::string getName(){return name;}
			inline Vector3 getRestPos(){return restPos;}
			inline Quaternion getRestRot(){return restRot;}
			inline Vector3 getRestScale(){return restScale;}
			inline Vector3 getPosePos(){return posePos;}
			inline Quaternion getPoseRot(){return poseRot;}
			inline Vector3 getPoseScale(){return poseScale;}
		private:
			float length;
			Bone *ikTarget = nullptr;
			int ikChainLength = -1;
			Skeleton *skeleton = nullptr;
			Vector3 initAxis[3], restPos, posePos = Vector3::VEC_ZERO, restScale, poseScale = Vector3::VEC_IJK;
			Quaternion restRot, poseRot = Quaternion::QUAT_W;
	};
}

#endif
