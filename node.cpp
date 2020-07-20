#include"root.h"
#include"node.h"
#include"mesh.h"
#include"particleEmitter.h"
#include"light.h"
#include"text.h"
#include"material.h"
#include"matrix.h"
#include<glm.hpp>
#include<glm/gtc/matrix_inverse.hpp>

using namespace std;
using namespace glm;

namespace vb01{
	Node::Node(Vector3 pos, Quaternion orientation, Vector3 scale,string name){
		this->pos=pos;
		this->scale=scale;
		this->orientation=orientation;
		this->name=name;
		axis[0]=Vector3::VEC_I;
		axis[1]=Vector3::VEC_J;
		axis[2]=Vector3::VEC_K;
	}

	Node::~Node(){
		for(Mesh *m : meshes)
			delete m;
		for(Light *l : lights)
			delete l;
		for(ParticleEmitter *p : emitters)
			delete p;
		for(Text *t : texts)
			delete t;
		for(Node *c : children){
			//dettachChild(c);
			delete c;
		}
	}

	void Node::update(){
		if(visible){
			for(Light *l : lights)
				l->update();
			for(Mesh *m : meshes)
				m->update();
			for(Text *t : texts)
				t->update();
			for(Node *c : children)
				c->update();
			for(ParticleEmitter *p : emitters)
				p->update();
		}
	}

	void Node::attachChild(Node *child){
		child->setParent(this);
		children.push_back(child);
		child->updateLocalAxis();
	}

	void Node::dettachChild(Node *child){
		child->setParent(nullptr);
		int id=-1;
		for(int i=0;i<children.size()&&id==-1;i++)
			if(children[i]==child)
				id=i;
		if(id!=-1)
			children.erase(children.begin()+id);
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
		Root::getSingleton()->numLights++;
		lights.push_back(light);	
		light->setNode(this);
		updateShaders();
	}

	void Node::removeLight(int id){
		Root::getSingleton()->numLights--;
		Light *light=lights[id];
		light->setNode(nullptr);
		lights.erase(lights.begin()+id);
		updateShaders();
	}

	void Node::addText(Text *text){
		texts.push_back(text);
		text->setNode(this);	
	}

	void Node::lookAt(Vector3 newDir,Vector3 newUp,Node *node){
		Vector3 parAxis[]{
			node->getLocalAxis(0),
			node->getLocalAxis(1),
			node->getLocalAxis(2)
		};
		newDir=parAxis[0]*newDir.x+parAxis[1]*newDir.y+parAxis[2]*newDir.z;
		newUp=parAxis[0]*newUp.x+parAxis[1]*newUp.y+parAxis[2]*newUp.z;
		/*
		*/
		float angle=axis[2].getAngleBetween(newDir);
		Vector3 rotAxis=axis[2].cross(newDir).norm();
		if(rotAxis==Vector3::VEC_ZERO)
			rotAxis=axis[1];
		//setOrientation((Quaternion(angle,rotAxis)*(orientation)));
		setOrientation(node->globalToLocalOrientation(Quaternion(angle,rotAxis)*node->localToGlobalOrientation(orientation)));

		mat3 mat;
		mat[0][0]=axis[0].x;
		mat[1][0]=axis[0].y;
		mat[2][0]=axis[0].z;
		mat[0][1]=axis[1].x;
		mat[1][1]=axis[1].y;
		mat[2][1]=axis[1].z;
		mat[0][2]=axis[2].x;
		mat[1][2]=axis[2].y;
		mat[2][2]=axis[2].z;
		mat=inverse(mat);
		vec3 nu=vec3(newUp.x,newUp.y,newUp.z)*mat;
		newUp=(axis[0]*nu.x+axis[1]*nu.y).norm();
		angle=axis[1].getAngleBetween(newUp);
		//setOrientation((Quaternion(angle*(nu.x<0?1:-1),axis[2])*(orientation)));
		setOrientation(node->globalToLocalOrientation(Quaternion(angle*(nu.x<0?1:-1),axis[2])*node->localToGlobalOrientation(orientation)));
		//setOrientation(node->globalToLocalOrientation(Quaternion(angle,axis[2])*node->localToGlobalOrientation(orientation)));
	}

	void Node::getDescendants(Node *node, vector<Node*> &descendants){
		for(int i=0;i<node->getNumChildren();i++){
			if(node->getChild(i)->getNumChildren()>0)
				getDescendants(node->getChild(i),descendants);
			else
				descendants.push_back(node->getChild(i));
		}
	}

	void Node::updateLocalAxis(){
		Node *rootNode=Root::getSingleton()->getRootNode(),*ancestor=this;
		while(ancestor->getParent())
			ancestor=ancestor->getParent();
		if(ancestor!=rootNode)
			return;

		Vector3 parAxis[3]={
			parent->getLocalAxis(0),
			parent->getLocalAxis(1),
			parent->getLocalAxis(2)
		};

		for(int i=0;i<3;i++)
			axis[i]=(orientation*parAxis[i]).norm();
		for(Node *ch : children)
			ch->updateLocalAxis();
	}

	Vector3 Node::localToGlobalPosition(Vector3 localPos){
		Vector3 origin=Vector3::VEC_ZERO;
		vector<Node*> ancestors;
		ancestors.push_back(this);
		Node *parent=this->getParent();

		while(parent){
			ancestors.push_back(parent);
			parent=parent->getParent();
		}

		while(!ancestors.empty()){
			int id=ancestors.size()-1;
			Node *par=ancestors[id]->getParent();
			Vector3 parAxis[]={
				par?par->getLocalAxis(0):Vector3::VEC_I,
				par?par->getLocalAxis(1):Vector3::VEC_J,
				par?par->getLocalAxis(2):Vector3::VEC_K
			};
			Vector3 p=ancestors[id]->getPosition();
			origin=origin+parAxis[0]*p.x+parAxis[1]*p.y+parAxis[2]*p.z;
			ancestors.pop_back();
		}
		return origin+axis[0]*localPos.x+axis[1]*localPos.y+axis[2]*localPos.z;
	}

	Vector3 Node::globalToLocalPosition(Vector3 globalPos){
		Vector3 origin=globalPos;
		vector<Node*> ancestors;
		ancestors.push_back(this);
		Node *parent=this->getParent();

		while(parent){
			ancestors.push_back(parent);
			parent=parent->getParent();
		}

		while(!ancestors.empty()){
			int id=0;
			Node *par=ancestors[id]->getParent();
			Vector3 parAxis[]={
				par?par->getLocalAxis(0):Vector3::VEC_I,
				par?par->getLocalAxis(1):Vector3::VEC_J,
				par?par->getLocalAxis(2):Vector3::VEC_K
			};
			Vector3 p=ancestors[id]->getPosition();
			origin=origin-parAxis[0]*p.x-parAxis[1]*p.y-parAxis[2]*p.z;
			ancestors.erase(ancestors.begin());
		}
		//return origin+axis[0]*localPos.x+axis[1]*localPos.y+axis[2]*localPos.z;
		return origin;
	}

	Quaternion Node::localToGlobalOrientation(Quaternion localRot){
		Quaternion origin=Quaternion::QUAT_W;
		vector<Node*> ancestors;
		ancestors.push_back(this);
		Node *parent=this->getParent();

		while(parent){
			ancestors.push_back(parent);
			parent=parent->getParent();
		}

		while(!ancestors.empty()){
			int id=ancestors.size()-1;
			Quaternion o=ancestors[id]->getOrientation();
			origin=o*origin;
			ancestors.pop_back();
		}
		return localRot*origin;
	}

	Quaternion Node::globalToLocalOrientation(Quaternion globalRot){
		Quaternion origin=globalRot;
		vector<Node*> ancestors;
		ancestors.push_back(this);
		Node *parent=this->getParent();

		while(parent){
			ancestors.push_back(parent);
			parent=parent->getParent();
		}

		while(!ancestors.empty()){
			int id=ancestors.size()-1;
			Quaternion o=ancestors[id]->getOrientation();
			origin=origin*Quaternion(-o.getAngle(),o.getAxis());
			ancestors.pop_back();
		}
		return origin;
	}

	void Node::updateShaders(){
		Root *root=Root::getSingleton();
		Node *rootNode=Root::getSingleton()->getRootNode();
		vector<Node*> descendants;
		rootNode->getDescendants(rootNode,descendants);
		descendants.push_back(rootNode);
		for(Node *n : descendants){
			vector<Mesh*> meshes=n->getMeshes();
			for(Mesh *m : meshes){
				Material *mat=m->getMaterial();
				string str="const int numLights="+to_string(root->numLights>0?root->numLights:1)+";";

				if(mat){
					//mat->getShader()->editShader(true,'=',';',str);
					mat->getShader()->editShader(Shader::FRAGMENT_SHADER,1,str);
				}
			}
		}
	}

	void Node::setOrientation(Quaternion q){
		this->orientation=q;
		updateLocalAxis();
	}
}
