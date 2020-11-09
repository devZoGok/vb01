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

	void AnimationChannel::update(){
		if(canUpdate()){
			if(firstFrame < currentFrame && currentFrame < numFrames)
				currentFrame += (forward? 1 : -1);
			else if(loop){
				if(!forward && currentFrame <= firstFrame)
					currentFrame = numFrames - 1;
				else if(forward && currentFrame >= numFrames)
					currentFrame = firstFrame + 1;
			}

			lastUpdateTime = getTime();
		}
	}

	void AnimationChannel::setAnimationName(string animName){
		this->animationName = animName;
		numFrames = getMaxKeyframeNum(animName);
	}

	int AnimationChannel::getMaxKeyframeNum(string animName){
		int maxKeyframeNum;

		Animation *animation = controller->getAnimation(animName);
		KeyframeGroup *startGroup = animation->getKeyframeGroup(bones[0]);
		int numStartKeyframes = startGroup->keyframeChannels[0].keyframes.size();
		maxKeyframeNum = startGroup->keyframeChannels[0].keyframes[numStartKeyframes - 1].frame;

		for(int i = 0; i < bones.size(); i++){
			KeyframeGroup *kg = animation->getKeyframeGroup(bones[i]);

			for(int j = 0 ; j < kg->keyframeChannels.size(); j++){
				KeyframeChannel ch = kg->keyframeChannels[j];

				for(int k = 0; k < ch.keyframes.size(); k++){
					int numKeyframes = ch.keyframes.size();
					int maxFrame = ch.keyframes[numKeyframes - 1].frame;

					if(numFrames < maxFrame)
						maxKeyframeNum = maxFrame;
				}
			}
		}

		return maxKeyframeNum;
	}
}
