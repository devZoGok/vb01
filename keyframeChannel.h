#ifndef KEYFRAME_CHANNEL_H
#define KEYFRAME_CHANNEL_H

#include <vector>
#include <string>

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
			SHAPE_KEY_MAX,
			DIFFUSE_COLOR_W,
			DIFFUSE_COLOR_X,
			DIFFUSE_COLOR_Y,
			DIFFUSE_COLOR_Z,
			SPECULAR_COLOR_W,
			SPECULAR_COLOR_X,
			SPECULAR_COLOR_Y,
			SPECULAR_COLOR_Z,
			TEXTURE_MIX_RATIO
		};
		struct Keyframe{
			enum Interpolation{CONSTANT, LINEAR, BEZIER};

			float value, leftHandleValue, rightHandleValue, frame, leftHandleFrame, rightHandleFrame;
			Interpolation interpolation;
		};

		Type type;
		Animatable *animatable = nullptr;
		std::vector<Keyframe> keyframes;

		static float interpolateBezier(std::vector<float>, float);
		static KeyframeChannel::Type getKeyframeChannelType(std::string);
		static float interpolate(Keyframe, Keyframe, float); 
		static Keyframe findKeyframe(float, KeyframeChannel, bool);
		static void transform();
	};

	typedef KeyframeChannel::Type KeyframeChannelType; 
	typedef KeyframeChannel::Keyframe Keyframe; 
	typedef KeyframeChannel::Keyframe::Interpolation KeyframeInterpolation; 
}

#endif
