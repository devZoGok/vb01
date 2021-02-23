#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include "keyframeChannel.h"

namespace vb01{
	class Animatable;

	class Animation{
		public:
			Animation(std::string);
			~Animation();
			void update();
			static KeyframeChannel::Type getKeyframeChannelType(std::string);
			KeyframeChannel* getKeyframeChannel(Animatable*, KeyframeChannel::Type);
			std::vector<KeyframeChannel> getKeyframeChannelsByAnimatable(Animatable*);
			inline void addKeyframeChannel(KeyframeChannel channel){keyframeChannels.push_back(channel);}
			inline std::string getName(){return name;}
			inline const std::vector<KeyframeChannel>& getKeyframeChannels(){return keyframeChannels;}
		private:
			std::string name;
			std::vector<KeyframeChannel> keyframeChannels;
	};
}

#endif
