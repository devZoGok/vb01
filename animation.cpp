#include"animation.h"

using namespace std;

namespace vb01{
	Animation::Animation(string name){
		this->name=name;
	}

	Animation::~Animation(){
	}

	void Animation::update(){
	}

	Animation::KeyframeGroup* Animation::getKeyframeGroup(Bone *b){
		int id=-1;
		for(int i=0;i<keyframeGroups.size();i++)
			if(keyframeGroups[i].bone==b){
				id=i;
				break;
			}
		return id>-1?&(keyframeGroups[id]):nullptr;
	}
}
