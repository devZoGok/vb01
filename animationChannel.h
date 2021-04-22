#ifndef ANIMATION_CHANNEL_H
#define ANIMATION_CHANNEL_H

#include <vector>
#include <string>

#include "animation.h"
#include "util.h"

namespace vb01{
	class AnimationController;
	class Bone;

	class AnimationChannel{
		public:
			AnimationChannel();
			~AnimationChannel();
			void update();
			void setAnimationName(std::string name);
			inline std::vector<Animatable*> getAnimatables(){return animatables;}
			inline void addAnimatable(Animatable *animatable){animatables.push_back(animatable);}
			inline void removeAllBones(){animatables.clear();}
			inline void setUpdateRate(float updateRate){this->updateRate = updateRate;}
			inline int getUpdateRate(){return updateRate;}
			inline int getFirstFrame(){return firstFrame;}
			inline int getCurrentFrame(){return currentFrame;}
			inline void setCurrentFrame(int frame){this->currentFrame = frame;}
			inline int getNumFrames(){return numFrames;}
			inline void setLoop(bool loop){this->loop = loop;}
			inline bool isLoop(){return loop;}
			inline void setForward(bool forward){this->forward = forward;}
			inline bool isForward(){return forward;}
			inline std::string getAnimationName(){return animationName;}
			inline void setAnimationController(AnimationController *controller){this->controller = controller;}
		private:
			inline bool canUpdate(){return getTime() - lastUpdateTime > updateRate;}
			int getMaxKeyframeNum(std::string);
			int getFirstFrameNum(std::string);
			std::vector<Keyframe> getAllKeyframes(std::string);

			s64 lastUpdateTime = 0;
		    int updateRate = 16;
			AnimationController *controller = nullptr;
			int firstFrame = 0, currentFrame = 1, numFrames = 0;
			bool loop = false, forward = true;
			std::string animationName;
			std::vector<Animatable*> animatables;

			friend class AnimationChannelTest;
			friend class AnimationControllerTest;
	};
}

#endif
