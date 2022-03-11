#include "driver.h"
#include "animatable.h"

#include <algorithm>

using namespace std;

namespace vb01{
	Driver::Driver(Animatable *animatable, KeyframeChannel keyframeChannel, VariableType type){
			this->animatable = animatable;
		this->keyframeChannel = keyframeChannel;
		this->type = type;
	}

	void Driver::drive(float driverValue){
		Keyframe pastKeyframe = KeyframeChannel::findKeyframe(driverValue, keyframeChannel, true);
		Keyframe nextKeyframe = KeyframeChannel::findKeyframe(driverValue, keyframeChannel, false);
		float pastFrame = pastKeyframe.frame;
		float nextFrame = nextKeyframe.frame;

		float ratio = (float)(driverValue - pastFrame) / (nextFrame - pastFrame);

		if(ratio < 0)
			ratio = 0;

		Keyframe::Interpolation interp = pastKeyframe.interpolation;
		float value = KeyframeChannel::interpolate(pastKeyframe, nextKeyframe, ratio);
		animatable->animate(value, keyframeChannel);
	}

	Driver::VariableType Driver::getDriverVariableType(string typeString){
		VariableType type;

		if(typeString == "LOC_X")
			type = Driver::POS_X;
		else if(typeString == "LOC_Y")
		   type = Driver::POS_Y;
		else if(typeString == "LOC_Z")
		   type = Driver::POS_Z;
		else if(typeString == "ROT_W")
		   type = Driver::ROT_W;
		else if(typeString == "ROT_X")
		   type = Driver::ROT_X;
		else if(typeString == "ROT_Y")
		   type = Driver::ROT_Y;
		else if(typeString == "ROT_Z")
		   type = Driver::ROT_Z;
		else if(typeString == "SCALE_X")
		   type = Driver::SCALE_X;
		else if(typeString == "SCALE_Y")
		   type = Driver::SCALE_Y;
		else if(typeString == "SCALE_Z")
			type = Driver::SCALE_Z;

		return type;
	}
}
