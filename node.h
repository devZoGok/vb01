#ifndef NODE_H
#define NODE_H

#include"quaternion.h"
#include"vector.h"
#include<vector>

namespace vb01{
	class Mesh;
	class Light;

	class Node{
		public:
			Node(Vector3);
			~Node();
			void setObject(Mesh*);
			void update();
			void attachChild(Node*);
			void addLight(Light*);
			inline Mesh* getMesh(){return mesh;}
			inline Node* getParent(){return parent;}
			inline Vector3 getPosition(){return pos;}
			inline std::vector<Node*>& getDescendants(){return descendants;}
			inline std::vector<Light*>& getLights(){return lights;}
			inline Light* getLight(int i){return lights[i];}
			inline int getNumLights(){return lights.size();}
		private:
			Vector3 pos;
			Mesh *mesh=nullptr;
			std::vector<Node*> children,descendants;
			std::vector<Light*> lights;
			Node *parent=nullptr;
	};
}

#endif
