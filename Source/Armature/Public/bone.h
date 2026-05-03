#ifndef BONE_H
#define BONE_H

#include "node.h"

#include <string>

namespace vb01{
	class Skeleton;

	class Bone : public Node{
		public:
			Bone(std::string, float, Vector3 = Vector3::VEC_ZERO, Quaternion = Quaternion::QUAT_W, Vector3 = Vector3::VEC_IJK);
			void setPosePos(Vector3 p);
			void setPoseRot(Quaternion r);
			void setPoseScale(Vector3 s);
			void lookAt(Vector3, Vector3);
			Vector3 getBoneSpaceRestPos(Bone*);
			Vector3 getModelSpacePos();
			inline Skeleton* getSkeleton(){return skeleton;}
			inline std::string getIkTarget(){return ikTarget;}
			inline void setIkTarget(std::string target){this->ikTarget = target;}
			inline int getIkChainLength(){return ikChainLength;}
			inline void setIkChainLength(int ikChainLength){this->ikChainLength = ikChainLength;}
			inline float getLength(){return length;}
			inline bool isIkFromTail(){return ikFromTail;}
			inline void setIkFromTail(bool ikFromTail){this->ikFromTail = ikFromTail;}
			inline void setSkeleton(Skeleton *sk){this->skeleton = sk;}
			inline Vector3 getInitAxis(int i){return initAxis[i];}
			inline Vector3 getRestPos(){return restPos;}
			inline Quaternion getRestRot(){return restRot;}
			inline Vector3 getRestScale(){return restScale;}
			inline Vector3 getPosePos(){return posePos;}
			inline Quaternion getPoseRot(){return poseRot;}
			inline Vector3 getPoseScale(){return poseScale;}
		private:
			void updateBoneInfo();
			void animate(float, KeyframeChannel);
			void onAttached();
			virtual float getDriverValue(Driver::VariableType);

			float length;
			std::string ikTarget = "";
			int ikChainLength = -1;
			bool ikFromTail = true;
			Skeleton *skeleton = nullptr;
			Vector3 initAxis[3], restPos, posePos = Vector3::VEC_ZERO, restScale, poseScale = Vector3::VEC_IJK;
			Quaternion restRot, poseRot = Quaternion::QUAT_W;
	};
}

#endif
