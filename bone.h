#ifndef BONE_H
#define BONE_H

#include"node.h"
#include<string>

namespace vb01{
	class Bone : public Node{
		public:
			Bone(std::string,Vector3=Vector3::VEC_ZERO);
			std::string getName(){return name;}
		private:
			std::string name;
			Vector3 yAxis,zAxis;
	};
}

#endif
