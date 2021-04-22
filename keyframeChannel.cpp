#include "keyframeChannel.h"

using namespace std;

namespace vb01{
	float KeyframeChannel::interpolate(Keyframe pastKeyframe, Keyframe nextKeyframe, float ratio){
		float currentValue;
		switch(pastKeyframe.interpolation){
			case Keyframe::CONSTANT:
				currentValue = pastKeyframe.value;
				break;
			case Keyframe::LINEAR:
				currentValue = interpolateBezier(vector<float>({pastKeyframe.value, nextKeyframe.value}), ratio);
				break;
			case Keyframe::BEZIER:
				vector<float> points = vector<float>({pastKeyframe.value, pastKeyframe.rightHandleValue, nextKeyframe.leftHandleValue, nextKeyframe.value});
				currentValue = interpolateBezier(points, ratio);
				break;
		}
		return currentValue;
	}

	float KeyframeChannel::interpolateBezier(vector<float> points, float ratio){
		int numPoints = points.size();
		vector<float> newPoints;
		for(int i = 0; i < numPoints - 1; i++)
			newPoints.push_back(points[i] + (points[i + 1] - points[i]) * ratio);
		
		if(newPoints.size() >= 2)
			return interpolateBezier(newPoints, ratio);
		else
			return newPoints[0];
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
		else if(typeString == "diffuse_color_x")
			type = KeyframeChannelType::DIFFUSE_COLOR_X;
		else if(typeString == "diffuse_color_y")
			type = KeyframeChannelType::DIFFUSE_COLOR_Y;
		else if(typeString == "diffuse_color_z")
			type = KeyframeChannelType::DIFFUSE_COLOR_Z;
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

	Keyframe KeyframeChannel::createKeyframe(KeyframeInterpolation interpolation, float frame, float value){
		Keyframe keyframe;
		keyframe.interpolation = interpolation;
		keyframe.frame = frame;
		keyframe.value = value;
		return keyframe;
	}
}
