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
			UNIFORM_1,
			UNIFORM_2,
			UNIFORM_3,
			UNIFORM_4,
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
			TEXTURE_FRAME_A,
			TEXTURE_FRAME_B,
			TEXTURE_MIX_RATIO,
			PARTICLE_EMITTER_SPEED,
			PARTICLE_EMITTER_SPREAD,
			PARTICLE_EMITTER_START_COLOR_W,
			PARTICLE_EMITTER_START_COLOR_X,
			PARTICLE_EMITTER_START_COLOR_Y,
			PARTICLE_EMITTER_START_COLOR_Z,
			PARTICLE_EMITTER_END_COLOR_W,
			PARTICLE_EMITTER_END_COLOR_X,
			PARTICLE_EMITTER_END_COLOR_Y,
			PARTICLE_EMITTER_END_COLOR_Z,
			PARTICLE_EMITTER_START_SIZE_X,
			PARTICLE_EMITTER_START_SIZE_Y,
			PARTICLE_EMITTER_END_SIZE_X,
			PARTICLE_EMITTER_END_SIZE_Y,
			PARTICLE_EMITTER_LOW_LIFE,
			PARTICLE_EMITTER_HIGH_LIFE,
			SPOTLIGHT_INNER_ANGLE,
			SPOTLIGHT_OUTER_ANGLE
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
		static Keyframe createKeyframe(Keyframe::Interpolation, float, float, float = 0, float = 0, float = 0, float = 0);
		static KeyframeChannel createKeyframeChannel(KeyframeChannel::Type, Animatable*, std::vector<Keyframe>);
	};

	typedef KeyframeChannel::Type KeyframeChannelType; 
	typedef KeyframeChannel::Keyframe Keyframe; 
	typedef KeyframeChannel::Keyframe::Interpolation KeyframeInterpolation; 
}

#endif
