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
	class Animatable;

	class AnimationController{
		public:
				static AnimationController* getSingleton();
			void update();
			Animation* getAnimation(std::string);
			void addAnimationChannel(AnimationChannel *channel);
			inline std::vector<Animation*> getAnimations(){return animations;}
			inline void addAnimation(Animation *anim){animations.push_back(anim);}
		private:
			std::vector<Animation*> animations;
			std::vector<AnimationChannel*> channels;

			AnimationController(){}
			void transform(AnimationChannel*);
		protected:
			void onAnimationEnd(std::string){}
			void onAnimationStart(std::string){}
	};
}

#endif
