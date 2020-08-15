#include"skeleton.h"
#include"animation.h"
#include"animationController.h"
#include"box.h"

using namespace std;

namespace vb01{
	Skeleton::Skeleton(string name){
		this->name=name;
		controller = new AnimationController(this);
	}

	void Skeleton::update(){
		for(Bone *b : bones)
			b->update();
	}

	void Skeleton::addBone(Bone *bone, Bone *parent){
		if(parent)
			parent->attachChild(bone);
		bone->setSkeleton(this);
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

}
