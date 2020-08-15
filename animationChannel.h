#ifndef ANIMATION_CHANNEL_H
#define ANIMATION_CHANNEL_H

#include <vector>
#include <string>

namespace vb01{
	class AnimationController;
	class Bone;

	class AnimationChannel{
		public:
			AnimationChannel(AnimationController*);
			~AnimationChannel();
			inline void setAnimationName(std::string name){this->animationName = name;}
			inline std::vector<Bone*> getBones(){return bones;}
			inline void addBone(Bone *bone){bones.push_back(bone);}
			inline void removeAllBones(){bones.clear();}
			inline void setSpeed(float speed){this->speed = speed;}
			inline float getSpeed(){return speed;}
			inline int getCurrentFrame(){return currentFrame;}
			inline void setCurrentFrame(int frame){this->currentFrame = frame;}
			inline void setLoop(bool loop){this->loop = loop;}
			inline bool isLoop(){return loop;}
			inline std::string getAnimationName(){return animationName;}
		private:
			AnimationController *controller = nullptr;
			float speed = 1.f;
			int currentFrame = 0;
			bool loop = false;
			std::string animationName;
			std::vector<Bone*> bones;
	};
}

#endif
