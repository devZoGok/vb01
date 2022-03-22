#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <string>

#include "bone.h"
#include "attachable.h"

namespace vb01{
	class Animation;

	class Skeleton : public Attachable{
		public:
			Skeleton(std::string name = "") : Attachable(name){}
			void update();
			void addBone(Bone*, Bone*);
			Bone* getBone(std::string);
			inline Bone* getBone(int i){return bones[i];}
			inline Bone* getRootBone(){return bones[0];}
			inline std::vector<Bone*>& getBones(){return bones;}
			inline int getNumBones(){return bones.size();}
		private:
			void updateIk();
			void solveIk(Bone*);
			void transformIkChain(int, Bone*[], Vector3[], Bone*);
			Bone** getIkBoneChain(Bone*);

			std::vector<Bone*> bones;
	};
}

#endif
