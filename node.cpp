#include"node.h"
#include"mesh.h"

namespace vb01{
	Node::Node(Vector3 pos){
		this->pos=pos;
	}

	Node::~Node(){}

	void Node::update(){
		if(mesh)
			mesh->update();			
		for(Node *c : children)
			c->update();
	}

	void Node::attachChild(Node *child){
		children.push_back(child);
	}

	void Node::setObject(Mesh *mesh){
		this->mesh=mesh;	
		mesh->setNode(this);
	}
}
