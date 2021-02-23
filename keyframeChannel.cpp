#include "keyframeChannel.h"

namespace vb01{
	float KeyframeChannel::interpolate(float pastValue, float nextValue, Keyframe::Interpolation mode, float ratio){
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

	Keyframe KeyframeChannel::findKeyframe(float time, KeyframeChannel keyframeChannel, bool last){
		int pastKeyframeId = -1;
		for(int i = 0; i < keyframeChannel.keyframes.size(); i++){
			Keyframe keyframe = keyframeChannel.keyframes[i];
			if(time >= keyframe.frame){
				pastKeyframeId = i;
			}
		}
		return keyframeChannel.keyframes[pastKeyframeId + (last ? 0 : 1)];
	}

	void KeyframeChannel::transform(){
	}
}
