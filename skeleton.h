#ifndef SKELETON_H
#define SKELETON_H

#include<vector>
#include<string>
#include"bone.h"

namespace vb01{
	class Animation;

	class Skeleton{
		public:
			Skeleton(std::string="");
			void addBone(Bone*,Node*);
			Bone* getBone(std::string);
			Animation* getAnimation(std::string);
			inline void addAnimation(Animation *anim){animations.push_back(anim);}
			inline Bone* getBone(int i){return bones[i];}
			inline std::vector<Bone*>& getBones(){return bones;}
			inline std::string getName(){return name;}
		private:
			std::string name;
			std::vector<Bone*> bones;
			std::vector<Animation*> animations;
	};
}

#endif
