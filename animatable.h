#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include "keyframeChannel.h"

namespace vb01{
	class Animatable{
		public:
			Animatable(std::string = "");
			virtual void animate(float, KeyframeChannel){}
			inline std::string getName(){return name;}
		protected:
			std::string name;
	};
}

#endif
