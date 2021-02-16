#include "root.h"
#include "node.h"
#include "mesh.h"
#include "particleEmitter.h"
#include "light.h"
#include "text.h"
#include "material.h"
#include "matrix.h"
#include "animationController.h"

#include <glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace std;
using namespace glm;

namespace vb01{
	Node::Node(Vector3 pos, Quaternion orientation, Vector3 scale, string name, AnimationController *controller){
		this->pos = pos;
		this->scale = scale;
		this->orientation = orientation;
		this->name = name;
		this->controller = controller;
		if(this->controller)
			this->controller->setNode(this);

		globalAxis[0] = Vector3::VEC_I;
		globalAxis[1] = Vector3::VEC_J;
		globalAxis[2] = Vector3::VEC_K;
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
			if(controller)
				controller->update();
		}
	}

	void Node::attachChild(Node *child){
		child->setParent(this);
		children.push_back(child);
		child->updateAxis();
	}

	void Node::dettachChild(Node *child){
		child->setParent(nullptr);
		int id = -1;
		for(int i = 0; i < children.size(); i++)
			if(children[i] == child){
				id = i;
				break;
			}
		if(id != -1)
			children.erase(children.begin() + id);
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
		Root::getSingleton()->shiftNumLights(true);
		lights.push_back(light);	
		light->setNode(this);
		updateShaders();
	}

	void Node::removeLight(int id){
		Root::getSingleton()->shiftNumLights(false);
		Light *light = lights[id];
		light->setNode(nullptr);
		lights.erase(lights.begin() + id);
		updateShaders();
	}

	void Node::addText(Text *text){
		texts.push_back(text);
		text->setNode(this);	
	}

	void Node::lookAt(Vector3 newDir, Vector3 newUp){
		adjustDir(newDir);
		adjustUp(newUp);
	}

	void Node::lookAt(Vector3 newDir){
		adjustDir(newDir);
	}

	void Node::adjustDir(Vector3 newDir){
		float angle = Vector3(0, 0, 1).getAngleBetween(newDir);
		Vector3 rotAxis = Vector3(0, 0, 1).cross(newDir).norm();
		if(rotAxis == Vector3::VEC_ZERO)
			rotAxis = Vector3::VEC_I;

		setOrientation(Quaternion(angle, rotAxis));
	}

	void Node::adjustUp(Vector3 newUp){
		Vector3 xDir =
		   	parent->globalToLocalPosition(localToGlobalPosition(Vector3::VEC_I)) - 
		   	parent->globalToLocalPosition(localToGlobalPosition(Vector3::VEC_ZERO));

		Vector3 yDir =
		   	parent->globalToLocalPosition(localToGlobalPosition(Vector3::VEC_J)) - 
		   	parent->globalToLocalPosition(localToGlobalPosition(Vector3::VEC_ZERO));

		Vector3 zDir = xDir.cross(yDir).norm();

		mat3 mat;
		mat[0][0] = xDir.x;
		mat[1][0] = xDir.y;
		mat[2][0] = xDir.z;
		mat[0][1] = yDir.x;
		mat[1][1] = yDir.y;
		mat[2][1] = yDir.z;
		mat[0][2] = zDir.x;
		mat[1][2] = zDir.y;
		mat[2][2] = zDir.z;

		vec3 nu = vec3(newUp.x, newUp.y, newUp.z) * inverse(mat);
		newUp = (xDir * nu.x + yDir * nu.y).norm();
		if(newUp != Vector3::VEC_ZERO){
			float angle = yDir.getAngleBetween(newUp);

			setOrientation(Quaternion(angle * (nu.x < 0 ? 1 : -1), zDir) * orientation);
		}
	}

	void Node::getDescendants(vector<Node*> &descendants){
		for(Node *child : getChildren()){
			descendants.push_back(child);
			if(!child->getChildren().empty())
				child->getDescendants(descendants);
		}
	}

	vector<Node*> Node::getAncestors(Node *topAncestor){
		vector<Node*> ancestors;
		Node *parent = this;
		while(parent){
			ancestors.push_back(parent);
			if(parent == topAncestor)
				break;
			parent = parent->getParent();
		}
		return ancestors;
	}

	void Node::updateAxis(){
		Vector3 parGlobalAxis[3]{
			parent->getGlobalAxis(0),
			parent->getGlobalAxis(1),
			parent->getGlobalAxis(2)
		};

		float rotAngle = orientation.getAngle();
		Vector3 rotAxis = orientation.getAxis();
		Vector3 newAxis = (parGlobalAxis[0] * rotAxis.x + parGlobalAxis[1] * rotAxis.y + parGlobalAxis[2] * rotAxis.z).norm();
		Quaternion rotQuat = Quaternion(rotAngle, newAxis);

		for(int i = 0; i < 3; i++)
			globalAxis[i] = (rotQuat * parGlobalAxis[i]).norm();

		for(Node *ch : children)
			ch->updateAxis();
	}

	Vector3 Node::localToGlobalPosition(Vector3 localPos){
		vector<Node*> ancestors = getAncestors();
		Vector3 origin = Vector3::VEC_ZERO;
		
		while(!ancestors.empty()){
	        int id = ancestors.size() - 1;
	        Node *par = ancestors[id]->getParent();
	        Vector3 parAxis[] = {
		        par ? par->getGlobalAxis(0) : Vector3::VEC_I,
		        par ? par->getGlobalAxis(1) : Vector3::VEC_J,
		        par ? par->getGlobalAxis(2) : Vector3::VEC_K
	        };
	        Vector3 p = ancestors[id]->getPosition();
	        origin = origin + parAxis[0] * p.x + parAxis[1] * p.y+parAxis[2] * p.z;
	        ancestors.pop_back();
        }

		return origin + globalAxis[0] * localPos.x + globalAxis[1] * localPos.y + globalAxis[2] * localPos.z;
	}

	Vector3 Node::globalToLocalPosition(Vector3 globalPos){
		Vector3 currentGlobalPos = localToGlobalPosition(Vector3::VEC_ZERO);

		mat3 mat;
		mat[0][0] = globalAxis[0].x;
		mat[1][0] = globalAxis[0].y;
		mat[2][0] = globalAxis[0].z;
		mat[0][1] = globalAxis[1].x;
		mat[1][1] = globalAxis[1].y;
		mat[2][1] = globalAxis[1].z;
		mat[0][2] = globalAxis[2].x;
		mat[1][2] = globalAxis[2].y;
		mat[2][2] = globalAxis[2].z;
		mat = inverse(mat);

		vec3 local = vec3(globalPos.x - currentGlobalPos.x, globalPos.y - currentGlobalPos.y, globalPos.z - currentGlobalPos.z) * mat;
		return Vector3(local.x, local.y, local.z);
	}

	Quaternion Node::adjustRot(vector<Node*> ancestors, Quaternion adjustableRot, bool localToGlobal){
		Quaternion rOrigin = adjustableRot;

		while(!ancestors.empty()){
			int id = localToGlobal ? 0 : ancestors.size() - 1;

			float angle = ancestors[id]->getOrientation().getAngle();
			Vector3 axis = ancestors[id]->getOrientation().getAxis();

			Quaternion o;
			if(localToGlobal){
				Node *par = ancestors[id]->getParent();
				Vector3 parAxis[]{
					par ? par->getGlobalAxis(0) : Vector3::VEC_I,
					par ? par->getGlobalAxis(1) : Vector3::VEC_J,
					par ? par->getGlobalAxis(2) : Vector3::VEC_K
				};

				axis = (parAxis[0] * axis.x + parAxis[1] * axis.y + parAxis[2] * axis.z).norm();

				o = Quaternion(angle, axis);
				rOrigin = rOrigin * o;
				ancestors.erase(ancestors.begin());
			}
			else{
				o = Quaternion(angle, axis).conj();
				rOrigin = rOrigin * o;
				ancestors.pop_back();
			}

		}

		return rOrigin;
	}

	Quaternion Node::localToGlobalOrientation(Quaternion localRot){
		vector<Node*> ancestors = getAncestors();

		Quaternion origin = adjustRot(ancestors, Quaternion::QUAT_W, true);

		return localRot * origin;
	}

	Quaternion Node::globalToLocalOrientation(Quaternion globalRot){
		vector<Node*> ancestors = getAncestors();
		
		Quaternion origin = adjustRot(ancestors, globalRot, false);

		return origin;
	}

	void Node::updateShaders(){
		Root *root = Root::getSingleton();

		Node *rootNode = root->getRootNode();
		vector<Node*> descendants;
		rootNode->getDescendants(descendants);
		descendants.push_back(rootNode);

		for(Node *n : descendants){
			vector<Mesh*> meshes = n->getMeshes();
			for(Mesh *m : meshes){
				Material *mat = m->getMaterial();
				int numLights = root->getNumLights();
				string str = "const int numLights = " + to_string(numLights > 0 ? numLights : 1) + ";";

				if(mat)
					mat->getShader()->editShader(Shader::FRAGMENT_SHADER, 1, str);
			}
		}
	}

	void Node::setOrientation(Quaternion q){
		this->orientation = q;
		updateAxis();
	}
}
