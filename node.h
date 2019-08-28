#ifndef NODE_H
#define NODE_H

#include"quaternion.h"
#include"vector.h"
#include<vector>

namespace vb01{
	class Mesh;

	class Node{
		public:
			Node(Vector3);
			~Node();
			void setObject(Mesh*);
			void update();
			void attachChild(Node*);
			inline Node* getParent(){return parent;}
			inline Vector3 getPosition(){return pos;}
		private:
			Vector3 pos;
			Mesh *mesh=nullptr;
			std::vector<Node*> children;
			Node *parent=nullptr;
	};
}

#endif
