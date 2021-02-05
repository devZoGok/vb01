#include "animationChannel.h"
#include "animationController.h"

using namespace std;

namespace vb01{
	AnimationChannel::AnimationChannel(AnimationController *controller){
		this->controller = controller;
	}

	AnimationChannel::~AnimationChannel(){
	}

	void AnimationChannel::update(){
		if(canUpdate()){
			if(firstFrame <= currentFrame && currentFrame <= numFrames)
				currentFrame += (forward ? 1 : -1);
			if(currentFrame < firstFrame)
				currentFrame = (loop ? numFrames : firstFrame);
			else if(currentFrame > numFrames)
				currentFrame = (loop ? firstFrame : numFrames);

			lastUpdateTime = getTime();
		}
	}

	void AnimationChannel::setAnimationName(string animName){
		this->animationName = animName;
		numFrames = getMaxKeyframeNum(animName);
		firstFrame = getFirstFrameNum(animName);
		currentFrame = firstFrame;
	}

	vector<Keyframe> AnimationChannel::getAllKeyframes(string animName){
		vector<Keyframe> keyframes;

		Animation *animation = controller->getAnimation(animName);
		KeyframeGroup *startGroup = animation->getKeyframeGroup(bones[0]);
		for(int i = 0; i < bones.size(); i++){
			KeyframeGroup *kg = animation->getKeyframeGroup(bones[i]);

			for(int j = 0 ; j < kg->keyframeChannels.size(); j++){
				KeyframeChannel ch = kg->keyframeChannels[j];

				for(int k = 0; k < ch.keyframes.size(); k++)
					keyframes.push_back(ch.keyframes[k]);
			}
		}
		return keyframes;
	}

	int AnimationChannel::getMaxKeyframeNum(string animName){
		vector<Keyframe> keyframes = getAllKeyframes(animName);
		int maxKeyframeNum = 0;

		for(int i = 0; i < keyframes.size(); i++){
			int curFrameNum = keyframes[i].frame;
			if(maxKeyframeNum < curFrameNum)
				maxKeyframeNum = curFrameNum;
		}

		return maxKeyframeNum;
	}

	int AnimationChannel::getFirstFrameNum(string animName){
		vector<Keyframe> keyframes = getAllKeyframes(animName);
		int minKeyframeNum = keyframes[0].frame;

		for(int i = 0; i < keyframes.size(); i++){
			int curFrameNum = keyframes[i].frame;
			if(minKeyframeNum > curFrameNum)
				minKeyframeNum = curFrameNum;
		}

		return minKeyframeNum;
	}
}
