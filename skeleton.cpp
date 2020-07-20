#include"skeleton.h"
#include"animation.h"
#include"box.h"

using namespace std;

namespace vb01{
	Skeleton::Skeleton(string name){
		this->name=name;
	}

	void Skeleton::addBone(Bone *bone, Node *parent){
		parent->attachChild(bone);
		bones.push_back(bone);

		Box *box=new Box(Vector3(.1,.1,.1));
		Material *mat=new Material();
		mat->setTexturingEnabled(false);
		mat->setLightingEnabled(false);
		mat->setDiffuseColor(Vector4(0,1,0,1));
		box->setMaterial(mat);
		//bone->attachMesh(box);
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
