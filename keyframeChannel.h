#ifndef KEYFRAME_CHANNEL_H
#define KEYFRAME_CHANNEL_H

#include <vector>

namespace vb01{
	class Animatable;

	struct KeyframeChannel{
		enum Type{
			POS_X,
			POS_Y,
			POS_Z,
			ROT_W,
			ROT_X,
			ROT_Y,
			ROT_Z,
			SCALE_X,
			SCALE_Y,
			SCALE_Z,
			SHAPE_KEY_MIN,
			SHAPE_KEY_VALUE,
			SHAPE_KEY_MAX
		};
		struct Keyframe{
			enum Interpolation{CONSTANT, LINEAR, BEZIER};

			float value, frame;
			Interpolation interpolation;
		};

		Type type;
		Animatable *animatable = nullptr;
		std::vector<Keyframe> keyframes;

		static float interpolate(float, float, Keyframe::Interpolation, float); 
		static Keyframe findKeyframe(float, KeyframeChannel, bool);
		static void transform();
	};

	typedef KeyframeChannel::Type KeyframeChannelType; 
	typedef KeyframeChannel::Keyframe Keyframe; 
	typedef KeyframeChannel::Keyframe::Interpolation KeyframeInterpolation; 
}

#endif
