#include"animation.h"

using namespace std;

namespace vb01{
	Animation::Animation(string name){
		this->name = name;
	}

	Animation::~Animation(){
	}

	void Animation::update(){
	}

	KeyframeChannel* Animation::getKeyframeChannel(Animatable *animatable, KeyframeChannelType type){
		KeyframeChannel *k = nullptr;

		for(KeyframeChannel &channel : keyframeChannels)
			if(channel.type == type && channel.animatable == animatable){
				k = &channel;
				break;
			}

		return k;
	}

	vector<KeyframeChannel> Animation::getKeyframeChannelsByAnimatable(Animatable *animatable){
		vector<KeyframeChannel> channels;

		for(KeyframeChannel channel : keyframeChannels)
			if(channel.animatable == animatable)
				channels.push_back(channel);

		return channels;
	}
}
