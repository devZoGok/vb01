#include"root.h"
#include"node.h"
#include"mesh.h"
#include"light.h"
#include"material.h"

using namespace std;

namespace vb01{
	Node::Node(Vector3 pos){
		this->pos=pos;
	}

	Node::~Node(){}

	void Node::update(){
		for(Mesh *m : meshes)
			m->update();
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

	void Node::attachMesh(Mesh *mesh){
		meshes.push_back(mesh);
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
		for(Node *n : descendants){
			vector<Mesh*> meshes=n->getMeshes();
			for(Mesh *m : meshes){
				Material *mat=m->getMaterial();
				if(mat) mat->getShader()->setNumLights(numLights>0?numLights:1);
			}
		}
	}
}
