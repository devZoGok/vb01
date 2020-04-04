#include"root.h"
#include"node.h"
#include"mesh.h"
#include"particleEmitter.h"
#include"light.h"
#include"text.h"
#include"material.h"
#include"matrix.h"

using namespace std;

namespace vb01{
	Node::Node(Vector3 pos, Quaternion orientation, Vector3 scale){
		this->pos=pos;
		this->scale=scale;
		this->orientation=orientation;
	}

	Node::~Node(){
		for(Light *l : lights)
			delete l;
		for(Mesh *m : meshes)
			delete m;
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
			/*
				*/
		}
	}

	void Node::attachChild(Node *child){
		child->setParent(this);
		children.push_back(child);
	}

	void Node::dettachChild(Node *child){
		child->setParent(nullptr);
		int id=-1;
		for(int i=0;i<children.size()&&id==-1;i++)
			if(children[i]==child)
				id=i;
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

	void Node::lookAt(Vector3 newDir,Vector3 upDir){
		Vector3 dir=getLocalAxis(2).norm(),newDirLocal=globalToLocal(newDir.norm()),upDirLocal=globalToLocal(upDir.norm());
		float angle=dir.getAngleBetween(newDirLocal);
		Vector3 rotAxis=dir.cross(newDirLocal);
		orientation=Quaternion(angle,rotAxis)*orientation;
		Vector3 up=getLocalAxis(1).norm();
		angle=up.getAngleBetween(upDirLocal);
		rotAxis=up.cross(upDirLocal);
		orientation=Quaternion(angle,rotAxis.norm())*orientation;
	}

	void Node::getDescendants(Node *node, vector<Node*> &descendants){
		for(int i=0;i<node->getNumChildren();i++){
			if(node->getChild(i)->getNumChildren()>0)
				getDescendants(node->getChild(i),descendants);
			else
				descendants.push_back(node->getChild(i));
		}
	}

	Node::Transform Node::getWorldTransform(){
		Transform t;
		Vector3 scale=Vector3::VEC_IJK;
		Quaternion orient=Quaternion::QUAT_W;
		Vector3 origin=Vector3::VEC_ZERO,axis[]={Vector3::VEC_I,Vector3::VEC_J,Vector3::VEC_K};
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
			Vector3 p=ancestors[id]->getPosition(),s=ancestors[id]->getScale();
			origin=origin+axis[0]*p.x*s.x+axis[1]*p.y*s.y+axis[2]*p.z*s.z;
			orient=o*orient;
			scale=s;
			for(int i=0;i<3;i++)
				axis[i]=orient*axis[i];
			ancestors.pop_back();
		}

		t.position=origin,t.orientation=orient,t.scale=scale;
		return t;
	}

	Vector3 Node::getLocalAxis(int i){
		Transform t;
		Vector3 pos=Vector3::VEC_ZERO,scale=Vector3::VEC_IJK;
		Quaternion orient=Quaternion::QUAT_W;
		Vector3 origin=Vector3::VEC_ZERO,axis[]={Vector3::VEC_I,Vector3::VEC_J,Vector3::VEC_K};
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
			Vector3 p=ancestors[id]->getPosition(),s=ancestors[id]->getScale();
			origin=origin+axis[0]*p.x*s.x+axis[1]*p.y*s.y+axis[2]*p.z*s.z;
			orient=o*orient;
			scale=s;
			for(int i=0;i<3;i++)
				axis[i]=orient*axis[i];
			ancestors.pop_back();
		}

		return axis[i];
	}

	Vector3 Node::localToGlobal(Vector3 p0){
		Vector3 dir=getLocalAxis(2),up=getLocalAxis(1),left=getLocalAxis(0);
		float **mat=new float*[3];
		for(int i=0;i<3;i++)
			mat[i]=new float[3];

		mat[0][0]=left.x;
		mat[1][0]=left.y;
		mat[2][0]=left.z;
		mat[0][1]=up.x;
		mat[1][1]=up.y;
		mat[2][1]=up.z;
		mat[0][2]=dir.x;
		mat[1][2]=dir.y;
		mat[2][2]=dir.z;

		Vector3 p=Matrix(mat,3,3)*p0;

		for(int i=0;i<3;i++)
			delete[] mat[i];
		delete[] mat;

		return p;
	}

	Vector3 Node::globalToLocal(Vector3 p0){
		Vector3 dir=getLocalAxis(2),up=getLocalAxis(1),left=getLocalAxis(0);
		float **mat=new float*[3];
		for(int i=0;i<3;i++)
			mat[i]=new float[3];

		mat[0][0]=left.x;
		mat[1][0]=left.y;
		mat[2][0]=left.z;
		mat[0][1]=up.x;
		mat[1][1]=up.y;
		mat[2][1]=up.z;
		mat[0][2]=dir.x;
		mat[1][2]=dir.y;
		mat[2][2]=dir.z;

		Matrix m(mat,3,3);
		m.invert();
		Vector3 p=m*p0;

		for(int i=0;i<3;i++)
			delete[] mat[i];
		delete[] mat;

		return p;
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
				string str=to_string(root->numLights>0?root->numLights:1);
				if(mat){
					//mat->getShader()->editShader(true,'=',';',str);
					mat->getShader()->editShader(false,'=',';',str);
				}
			}
		}
	}
}
