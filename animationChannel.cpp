#include "animationChannel.h"
#include "animationController.h"
#include "animation.h"

using namespace std;

namespace vb01{
	AnimationChannel::AnimationChannel(AnimationController *controller){
		this->controller = controller;
	}

	AnimationChannel::~AnimationChannel(){
	}

	void AnimationChannel::setAnimationName(string animName){
		this->animationName = animName;

		Animation *animation = controller->getAnimation(animName);
		Animation::KeyframeGroup *startGroup = animation->getKeyframeGroup(bones[0]);
		int maxStartFrame = startGroup->keyframes[startGroup->keyframes.size() - 1].frame;
		for(int i = 0; i < bones.size() ; i++){
			Animation::KeyframeGroup *group = animation->getKeyframeGroup(bones[i]);
			int maxFrame = group->keyframes[group->keyframes.size() - 1].frame;
			if(maxStartFrame < maxFrame)
				maxStartFrame = maxFrame;
		}
	}
}
