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

	Animation::KeyframeGroup* Animation::getKeyframeGroup(Node *b){
		int id = -1;
		for(int i = 0;i < keyframeGroups.size(); i++)
			if(keyframeGroups[i].bone == b){
				id = i;
				break;
			}
		return id > -1 ? &(keyframeGroups[id]) : nullptr;
	}

	KeyframeChannelType Animation::getKeyframeChannelType(string typeString){
		KeyframeChannelType type;
		if(typeString == "pos_x")
			type = KeyframeChannelType::POS_X;
		else if(typeString == "pos_y")
			type = KeyframeChannelType::POS_Y;
		else if(typeString == "pos_z")
			type = KeyframeChannelType::POS_Z;
		else if(typeString == "rot_w")
			type = KeyframeChannelType::ROT_W;
		else if(typeString == "rot_x")
			type = KeyframeChannelType::ROT_X;
		else if(typeString == "rot_y")
			type = KeyframeChannelType::ROT_Y;
		else if(typeString == "rot_z")
			type = KeyframeChannelType::ROT_Z;
		else if(typeString == "scale_x")
			type = KeyframeChannelType::SCALE_X;
		else if(typeString == "scale_y")
			type = KeyframeChannelType::SCALE_Y;
		else if(typeString == "scale_z")
			type = KeyframeChannelType::SCALE_Z;
		return type;
	}

	KeyframeChannel* Animation::getKeyframeChannel(Node *bone, KeyframeChannelType type){
		KeyframeChannel *k = nullptr;

		KeyframeGroup *group = getKeyframeGroup(bone);
		for(KeyframeChannel &channel : group->keyframeChannels){
			if(channel.type == type){
				k = &channel;
				break;
			}
		}

		return k;
	}
}
