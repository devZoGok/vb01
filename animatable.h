#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include "animation.h"

namespace vb01{
	class AnimationController;

	class Animatable{
		public:
			enum Type{NODE, BONE};

			Animatable();
			virtual void animate(float, KeyframeChannel){}
			inline Type getType(){return type;}
		private:
		protected:
			Type type;
	};
}

#endif
