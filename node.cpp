#include"root.h"
#include"node.h"
#include"mesh.h"
#include"particleEmitter.h"
#include"light.h"
#include"text.h"
#include"material.h"

using namespace std;

namespace vb01{
	Node::Node(Vector3 pos, Quaternion orientation, Vector3 scale){
		this->pos=pos;
		this->scale=scale;
		this->orientation=orientation;
	}

	Node::~Node(){}

	void Node::update(){
		for(Light *l : lights)
			l->update();
		for(Mesh *m : meshes)
			m->update();
		for(ParticleEmitter *p : emitters)
			p->update();
		for(Text *t : texts)
			t->update();
		for(Node *c : children)
			c->update();
	}

	void Node::attachChild(Node *child){
		/*
		Node *parent=getParent();
		while(parent){
			parent->getDescendants().push_back(child);
			parent=parent->getParent();
		}
		descendants.push_back(child);
		*/
		child->setParent(this);
		children.push_back(child);
	}

	void Node::attachMesh(Mesh *mesh){
		meshes.push_back(mesh);
		mesh->setNode(this);
	}

	void Node::attachParticleEmitter(ParticleEmitter *emitter){
		emitters.push_back(emitter);
		emitter->setNode(this);
	}

	void Node::addLight(Light *light){
		lights.push_back(light);	
		light->setNode(this);

		Node *rootNode=Root::getSingleton()->getRootNode();
		vector<Node*> descendants;
		rootNode->getDescendants(rootNode,descendants);
		descendants.push_back(rootNode);
		int numLights=0;
		for(Node *n : descendants)
			numLights+=n->getNumLights();
		for(Node *n : descendants){
			vector<Mesh*> meshes=n->getMeshes();
			for(Mesh *m : meshes){
				Material *mat=m->getMaterial();
				string str=to_string(numLights>0?numLights:1);
				if(mat){
					mat->getShader()->editShader(true,'=',';',str);
					mat->getShader()->editShader(false,'=',';',str);
				}
			}
		}
	}

	void Node::addText(Text *text){
		texts.push_back(text);
		text->setNode(this);	
	}

	void Node::getDescendants(Node *node, vector<Node*> &descendants){
		for(int i=0;i<node->getNumChildren();i++){
			if(node->getChild(i)->getNumChildren()>0)
				getDescendants(node->getChild(i),descendants);
			else
				descendants.push_back(node->getChild(i));
		}
	}
}
