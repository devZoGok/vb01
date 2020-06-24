#include"bone.h"

using namespace std;

namespace vb01{
	Bone::Bone(string name, Vector3 pos) : Node(pos){
		this->name=name;
	}
}
