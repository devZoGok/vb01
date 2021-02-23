#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include "animation.h"

namespace vb01{
	class AnimationController;

	class Animatable{
		public:
			Animatable();
			virtual void animate(float, KeyframeChannel){}
		private:
		protected:
	};
}

#endif
