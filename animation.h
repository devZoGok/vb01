#ifndef ANIMATION_H
#define ANIMATION_H

#include<string>
#include<vector>

namespace vb01{
	class Bone;

	class Animation{
		public:
			struct KeyframeGroup{
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
						SCALE_Z
					};
					struct Keyframe{
						enum Interpolation{CONSTANT, LINEAR, BEZIER};

						float value;
						int frame;
						Interpolation interpolation;
					};

					Type type;
					std::vector<Keyframe> keyframes;
				};

				Bone *bone = nullptr;
				std::vector<KeyframeChannel> keyframeChannels;
			};


			Animation(std::string);
			~Animation();
			void update();
			KeyframeGroup* getKeyframeGroup(Bone *b);
			KeyframeGroup::KeyframeChannel* getKeyframeChannel(Bone *b, KeyframeGroup::KeyframeChannel::Type);
			static KeyframeGroup::KeyframeChannel::Type getKeyframeChannelType(std::string&);
			inline int getNumKeyframeGroups(){return keyframeGroups.size();}
			inline std::string getName(){return name;}
			inline void addKeyframeGroup(KeyframeGroup k){keyframeGroups.push_back(k);}
		private:
			std::string name;
			std::vector<KeyframeGroup> keyframeGroups;
	};

	typedef Animation::KeyframeGroup KeyframeGroup; 
	typedef Animation::KeyframeGroup::KeyframeChannel KeyframeChannel; 
	typedef Animation::KeyframeGroup::KeyframeChannel::Type KeyframeChannelType; 
	typedef Animation::KeyframeGroup::KeyframeChannel::Keyframe Keyframe; 
	typedef Animation::KeyframeGroup::KeyframeChannel::Keyframe::Interpolation KeyframeInterpolation; 
}

#endif
