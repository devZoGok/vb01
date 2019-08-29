#include"root.h"
#include"node.h"
#include"mesh.h"
#include"light.h"

using namespace std;

namespace vb01{
	Node::Node(Vector3 pos){
		this->pos=pos;
	}

	Node::~Node(){}

	void Node::update(){
		if(mesh)
			mesh->update();			
		for(Light *l : lights)
			l->update();
		for(Node *c : children)
			c->update();
	}

	void Node::attachChild(Node *child){
		Node *parent=getParent();
		while(parent){
			parent->getDescendants().push_back(child);
			parent=parent->getParent();
		}
		children.push_back(child);
		descendants.push_back(child);
	}

	void Node::setObject(Mesh *mesh){
		this->mesh=mesh;	
		mesh->setNode(this);
	}

	void Node::addLight(Light *light){
		lights.push_back(light);	
		light->setNode(this);

		Node *rootNode=Root::getSingleton()->getRootNode();
		vector<Node*> descendants=rootNode->getDescendants();
		descendants.push_back(rootNode);
		int numLights=0;
		for(Node *n : descendants)
			numLights+=n->getNumLights();
		for(Node *n : descendants)
			if(n->getMesh())
				if(n->getMesh()->getMaterial())
					n->getMesh()->getMaterial()->getShader()->setNumLights(numLights>0?numLights:1);
	}
}
