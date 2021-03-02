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

	void AnimationController::transform(AnimationChannel *channel){
		Animation *animation = getAnimation(channel->getAnimationName());
		for(Animatable *animatable : channel->getAnimatables()){
			vector<KeyframeChannel> keyframeChannels = animation->getKeyframeChannelsByAnimatable(animatable);
			for(KeyframeChannel keyframeChannel : keyframeChannels){
				int currentFrame = channel->getCurrentFrame();
				Keyframe pastKeyframe = KeyframeChannel::findKeyframe(currentFrame, keyframeChannel, true);
				Keyframe nextKeyframe = KeyframeChannel::findKeyframe(currentFrame, keyframeChannel, false);
				int pastFrame = pastKeyframe.frame;
				int nextFrame = nextKeyframe.frame;

				float ratio = (float)(max(0, currentFrame - pastFrame)) / (nextFrame - pastFrame);
				Keyframe::Interpolation interp = pastKeyframe.interpolation;
				float value = KeyframeChannel::interpolate(pastKeyframe, nextKeyframe, ratio);
				animatable->animate(value, keyframeChannel);
			}
		}
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
