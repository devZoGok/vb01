#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include<string>

namespace vb01{
	class AnimationController{
		public:
			AnimationController();
			~AnimationController();
			void update();
		private:
		protected:
			void onAnimationEnd(std::string){}
			void onAnimationStart(std::string){}
	};
}

#endif
