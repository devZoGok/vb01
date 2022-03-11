#include "animation.h"

using namespace std;

namespace vb01{
	Animation::Animation(string name){
		this->name = name;
	}

	KeyframeChannel* Animation::getKeyframeChannel(string animatable, KeyframeChannelType type){
		KeyframeChannel *k = nullptr;

		for(KeyframeChannel &channel : keyframeChannels)
			if(channel.type == type && channel.animatable == animatable){
				k = &channel;
				break;
			}

		return k;
	}

	vector<KeyframeChannel> Animation::getKeyframeChannelsByAnimatable(string animatable){
		vector<KeyframeChannel> channels;

		for(KeyframeChannel channel : keyframeChannels)
			if(channel.animatable == animatable)
				channels.push_back(channel);

		return channels;
	}
}
