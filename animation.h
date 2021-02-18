#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>

namespace vb01{
	class Bone;
	class Node;

	class Animation{
		public:
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
					VALUE
				};
				struct Keyframe{
					enum Interpolation{CONSTANT, LINEAR, BEZIER};

					float value;
					int frame;
					Interpolation interpolation;
				};

				Type type;
				Node *bone = nullptr;
				std::vector<Keyframe> keyframes;
			};

			Animation(std::string);
			~Animation();
			void update();
			static KeyframeChannel::Type getKeyframeChannelType(std::string);
			KeyframeChannel* getKeyframeChannel(Node*, KeyframeChannel::Type);
			std::vector<KeyframeChannel> getKeyframeChannelsByNode(Node*);
			inline void addKeyframeChannel(KeyframeChannel channel){keyframeChannels.push_back(channel);}
			inline std::string getName(){return name;}
			inline const std::vector<KeyframeChannel>& getKeyframeChannels(){return keyframeChannels;}
		private:
			std::string name;
			std::vector<KeyframeChannel> keyframeChannels;
	};

	typedef Animation::KeyframeChannel KeyframeChannel; 
	typedef Animation::KeyframeChannel::Type KeyframeChannelType; 
	typedef Animation::KeyframeChannel::Keyframe Keyframe; 
	typedef Animation::KeyframeChannel::Keyframe::Interpolation KeyframeInterpolation; 
}

#endif
