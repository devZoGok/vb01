#ifndef ANIMATION_H
#define ANIMATION_H

#include<string>
#include<vector>

namespace vb01{
	class Bone;

	class Animation{
		public:
			struct KeyframeGroup{
				struct Keyframe{
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
					enum Interpolation{CONSTANT,LINEAR,BEZIER};
	
					Type type;
					Interpolation interpolation;
					float value;
					int frame;
				};
				Bone *bone=nullptr;
				std::vector<Keyframe> keyframes;
			};

			Animation(std::string);
			~Animation();
			void update();
			KeyframeGroup* getKeyframeGroup(Bone *b);
			inline std::string getName(){return name;}
			inline void addKeyframeGroup(KeyframeGroup k){keyframeGroups.push_back(k);}
		private:
			std::string name;
			std::vector<KeyframeGroup> keyframeGroups;
	};
}

#endif
