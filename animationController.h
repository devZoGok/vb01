#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <string>
#include <vector>
#include "animation.h"

namespace vb01{
	class Skeleton;
	class AnimationChannel;
	class Vector3;
	class Quaternion;

	class AnimationController{
		public:
			AnimationController();
			~AnimationController();
			void update();
			Animation* getAnimation(std::string);
			inline std::vector<Animation*> getAnimations(){return animations;}
			inline void addAnimation(Animation *anim){animations.push_back(anim);}
			inline void addAnimationChannel(AnimationChannel *channel){channels.push_back(channel);}
			inline void setSkeleton(Skeleton *sk){this->skeleton = sk;}
			inline void setNode(Node *node){this->node = node;}
			inline Skeleton* getSkeleton(){return skeleton;}
			inline Node* getNode(){return node;}
		private:
			Skeleton *skeleton = nullptr;
			Node *node = nullptr;
			std::vector<Animation*> animations;
			std::vector<AnimationChannel*> channels;

			void transform(AnimationChannel*);
			void setFrameValue(float, KeyframeChannelType, Vector3&, Quaternion&, Vector3&); 
			Keyframe findKeyframe(AnimationChannel*, KeyframeChannel, bool);
			float interpolate(float, float, Keyframe::Interpolation, float);
		protected:
			void onAnimationEnd(std::string){}
			void onAnimationStart(std::string){}
	};
}

#endif
