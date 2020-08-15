#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <string>
#include <vector>
#include "animation.h"

namespace vb01{
	class Skeleton;
	class AnimationChannel;

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
		protected:
			void onAnimationEnd(std::string){}
			void onAnimationStart(std::string){}
	};
}

#endif
