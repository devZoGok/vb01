#include "keyframeChannel.h"

using namespace std;

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

	KeyframeChannelType KeyframeChannel::getKeyframeChannelType(string typeString){
		KeyframeChannelType type;
		if(typeString == "pos_x")
			type = KeyframeChannelType::POS_X;
		else if(typeString == "pos_y")
			type = KeyframeChannelType::POS_Y;
		else if(typeString == "pos_z")
			type = KeyframeChannelType::POS_Z;
		else if(typeString == "rot_w")
			type = KeyframeChannelType::ROT_W;
		else if(typeString == "rot_x")
			type = KeyframeChannelType::ROT_X;
		else if(typeString == "rot_y")
			type = KeyframeChannelType::ROT_Y;
		else if(typeString == "rot_z")
			type = KeyframeChannelType::ROT_Z;
		else if(typeString == "scale_x")
			type = KeyframeChannelType::SCALE_X;
		else if(typeString == "scale_y")
			type = KeyframeChannelType::SCALE_Y;
		else if(typeString == "scale_z")
			type = KeyframeChannelType::SCALE_Z;
		else if(typeString == "shape_key_min")
			type = KeyframeChannelType::SHAPE_KEY_MIN;
		else if(typeString == "shape_key_value")
			type = KeyframeChannelType::SHAPE_KEY_VALUE;
		else if(typeString == "shape_key_max")
			type = KeyframeChannelType::SHAPE_KEY_MAX;
		return type;
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
