#include "driver.h"
#include "animatable.h"

#include <algorithm>

using namespace std;

namespace vb01{
	Driver::Driver(){}

	void Driver::drive(float driverValue){
		Keyframe pastKeyframe = KeyframeChannel::findKeyframe(driverValue, keyframeChannel, true);
		Keyframe nextKeyframe = KeyframeChannel::findKeyframe(driverValue, keyframeChannel, false);
		float pastFrame = pastKeyframe.frame;
		float nextFrame = nextKeyframe.frame;

		float ratio = (float)(driverValue - pastFrame) / (nextFrame - pastFrame);
		if(ratio < 0)
			ratio = 0;

		Keyframe::Interpolation interp = pastKeyframe.interpolation;
		float value = KeyframeChannel::interpolate(pastKeyframe.value, nextKeyframe.value, interp, ratio);
		target->animate(value, keyframeChannel);
	}
}
