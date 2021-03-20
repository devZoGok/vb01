#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include "keyframeChannel.h"

namespace vb01{
	class Animatable{
		public:
			Animatable(){}
			virtual void animate(float, KeyframeChannel){}
	};
}

#endif
