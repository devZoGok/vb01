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
			AnimationController(Skeleton*);
			~AnimationController();
			void update();
			Animation* getAnimation(std::string);
			inline void addAnimation(Animation *anim){animations.push_back(anim);}
			inline void addAnimationChannel(AnimationChannel *channel){channels.push_back(channel);}
		private:
			Skeleton *skeleton = nullptr;
			std::vector<Animation*> animations;
			std::vector<AnimationChannel*> channels;

			void transformBones(AnimationChannel*);
			void prepareAdjacentValues(Vector3&, Vector3&, Quaternion&, Quaternion&, Vector3&, Vector3&, Keyframe::Interpolation&, float&, KeyframeGroup*, AnimationChannel*); 
			Vector3 interpolate(Vector3, Vector3, Keyframe::Interpolation, float);
			Quaternion interpolate(Quaternion, Quaternion, Keyframe::Interpolation, float);
		protected:
			void onAnimationEnd(std::string){}
			void onAnimationStart(std::string){}
	};
}

#endif
