#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include "animation.h"

namespace vb01{
	class AnimationController;

	class Animatable{
		public:
			enum Type{NODE, BONE};

			Animatable(AnimationController*);
			void update();
			virtual void animate(float, KeyframeChannel){}
			inline AnimationController *getAnimationController(){return animationController;}
			inline Type getType(){return type;}
		private:
			AnimationController *animationController = nullptr;
		protected:
			Type type;
	};
}

#endif
