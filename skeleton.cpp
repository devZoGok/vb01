#include"skeleton.h"
#include"animation.h"

using namespace std;

namespace vb01{
	Skeleton::Skeleton(string name){
		this->name=name;
	}

	void Skeleton::addBone(Bone *bone, Node *parent){
		parent->attachChild(bone);
		bones.push_back(bone);
	}

	Bone* Skeleton::getBone(string name){
		Bone *bone=nullptr;
		for(Bone *b : bones)
			if(b->getName()==name){
				bone=b;
				break;
			}
		return bone;
	}

	Animation* Skeleton::getAnimation(string name){
		Animation *anim=nullptr;
		for(Animation *a : animations)
			if(a->getName()==name){
				anim=a;
				break;
			}
		return anim;
	}
}
