#include "animationController.h"
#include "animationChannel.h"
#include "skeleton.h"

using namespace std;

namespace vb01{
	AnimationController::AnimationController(){
	}

	AnimationController::~AnimationController(){
	}

	void AnimationController::update(){
		for(AnimationChannel *channel : channels){
			channel->update();

			transform(channel);
		}
	}

	Keyframe AnimationController::findKeyframe(AnimationChannel *channel, KeyframeChannel keyframeChannel, bool last){
		int pastKeyframeId = -1;
		for(int i = 0; i < keyframeChannel.keyframes.size(); i++){
			Keyframe keyframe = keyframeChannel.keyframes[i];
			if(channel->getCurrentFrame() >= keyframe.frame){
				pastKeyframeId = i;
			}
		}
		return keyframeChannel.keyframes[pastKeyframeId + (last ? 0 : 1)];
	}

	void AnimationController::transform(AnimationChannel *channel){
		Animation *animation = getAnimation(channel->getAnimationName());
		for(Animatable *animatable : channel->getAnimatables()){
			vector<KeyframeChannel> keyframeChannels = animation->getKeyframeChannelsByAnimatable(animatable);
			for(KeyframeChannel keyframeChannel : keyframeChannels){
				Keyframe pastKeyframe = findKeyframe(channel, keyframeChannel, true);
				Keyframe nextKeyframe = findKeyframe(channel, keyframeChannel, false);
				int pastFrame = pastKeyframe.frame;
				int nextFrame = nextKeyframe.frame;
				float ratio = (float)(max(0, channel->getCurrentFrame() - pastFrame)) / (nextFrame - pastFrame);
				Keyframe::Interpolation interp = pastKeyframe.interpolation;
				float value = interpolate(pastKeyframe.value, nextKeyframe.value, interp, ratio);
				animatable->animate(value, keyframeChannel);
			}
		}
	}

	float AnimationController::interpolate(float pastValue, float nextValue, Keyframe::Interpolation mode, float ratio){
		float currentValue;
		switch(mode){
			case Keyframe::CONSTANT:
				currentValue = pastValue;
				break;
			case Keyframe::LINEAR:
				currentValue = pastValue + (nextValue - pastValue) * ratio;
				break;
			case Keyframe::BEZIER:
				currentValue = pastValue + (nextValue - pastValue) * ratio;
				break;
		}
		return currentValue;
	}

	Animation* AnimationController::getAnimation(string name){
		Animation *anim = nullptr;
		for(Animation *a : animations)
			if(a->getName() == name){
				anim = a;
				break;
			}
		return anim;
	}
}
