#include "root.h"
#include "bone.h"
#include "mesh.h"
#include "text.h"
#include "util.h"
#include "light.h"
#include "matrix.h"
#include "material.h"
#include "skeleton.h"
#include "shaderAsset.h"
#include "particleEmitter.h"
#include "assetManager.h"
#include "animationController.h"

#include <glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace std;
using namespace glm;

namespace vb01{
	Node::Node(Vector3 pos, Quaternion orientation, Vector3 scale, string name, Animatable::Type type) : Animatable(type, name){
		this->pos = pos;
		this->scale = scale;
		this->orientation = orientation;

		if(type == Animatable::NONE)
				type = Animatable::NODE;
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

		for(Node *c : children)
			delete c;
	}

	void Node::update(){
		bool render = true;
		Node *ancestor = this;

		while(ancestor){
			render = ancestor->isVisible();
			ancestor = ancestor->getParent();

			if(!render) break;
		}

		for(Light *l : lights)
			l->update(render);

		if(render){
			for(Mesh *m : meshes)
				m->update();

			for(Text *t : texts)
				t->update();

			for(ParticleEmitter *p : emitters)
				p->update();

			for(Skeleton *sk : skeletons)
					sk->update();
		}

		for(Driver *driver : drivers)
			driver->drive(getDriverValue(driver->getType()));
	}

	Node* Node::clone(){
			vector<Node*> originalDescendants = vector<Node*>{this};
			getDescendants(originalDescendants);

			int numDescendants = originalDescendants.size();
			vector<Node*> clonedDescendants;

			for(int i = 0; i < numDescendants; i++){
					Node *original = originalDescendants[i];
					Bone *originalBone = dynamic_cast<Bone*>(original);
					Node *clone = nullptr;

					if(originalBone){
						Bone *boneClone = new Bone(originalBone->getName(), originalBone->getLength(), originalBone->getPosition(), originalBone->getOrientation(), originalBone->getScale());
						boneClone->setIkChainLength(originalBone->getIkChainLength());
						boneClone->setIkTarget(originalBone->getIkTarget());
						clone = boneClone;
					}
					else
						clone = new Node(original->getPosition(), original->getOrientation(), original->getScale(), original->getName());

					for(int j = 0; j < i; j++)
							if(original->getParent() == originalDescendants[j]){
								clonedDescendants[j]->attachChild(clone);
								break;
							}

					clonedDescendants.push_back(clone);
			}

			for(int i = 0; i < numDescendants; i++){
					Node *original = originalDescendants[i];

					for(Skeleton *skel : original->getSkeletons()){
							Skeleton *sk = new Skeleton(skel->getAttachableName());

							for(Bone *bone : skel->getBones())
									for(int j = 0; j < numDescendants; j++)
											if(bone == originalDescendants[j]){
													sk->addBone(dynamic_cast<Bone*>(clonedDescendants[j]), nullptr);
													break;
											}

							clonedDescendants[i]->addSkeleton(sk);
					}

					for(Mesh *mesh : original->getMeshes()){
							Mesh *m = new Mesh(MeshData(mesh->getMeshBase()));

							if(mesh->getMeshBase().fullSkeletonName != "")
									for(int j = 0; j < numDescendants; j++)
											for(int k = 0; k < clonedDescendants[j]->getNumSkeletons(); k++)
													if(
																	clonedDescendants[j]->getName() == originalDescendants[j]->getName() &&
																 	clonedDescendants[j]->getSkeleton(k)->getAttachableName() == mesh->getMeshBase().fullSkeletonName
													){
															m->setSkeleton(clonedDescendants[j]->getSkeleton(k));
															break;
													}

							clonedDescendants[i]->attachMesh(m);
					}
			}

			for(int i = 0; i < numDescendants; i++){
					for(Driver *driver : originalDescendants[i]->getDrivers()){
							Animatable *animatable = nullptr;

							for(int j = 0; j < numDescendants; j++){
									if(originalDescendants[j] == driver->getAnimatable()){
											animatable = clonedDescendants[j];
											break;
									}

									for(int k = 0; k < originalDescendants[j]->getNumMeshes(); k++){
											for(int l = 0; l < originalDescendants[j]->getMesh(k)->getMeshBase().numShapeKeys; l++)
													if(originalDescendants[j]->getMesh(k)->getShapeKey(l)->name == driver->getAnimatable()->getName()){
															animatable = clonedDescendants[j]->getMesh(k)->getShapeKey(l);
															break;
													}
									}
							}

							Driver *dr = new Driver(animatable, driver->getKeyframeChannel(), driver->getType());
							clonedDescendants[i]->addDriver(dr);
					}
			}

			return clonedDescendants[0];
	}

	float Node::getDriverValue(Driver::VariableType type){
		float driverValue;

		switch(type){
			case Driver::POS_X:
				driverValue = pos.x;
				break;
			case Driver::POS_Y:
				driverValue = pos.y;
				break;
			case Driver::POS_Z:
				driverValue = pos.z;
				break;
			case Driver::ROT_W:
				driverValue = orientation.w;
				break;
			case Driver::ROT_X:
				driverValue = orientation.x;
				break;
			case Driver::ROT_Y:
				driverValue = orientation.y;
				break;
			case Driver::ROT_Z:
				driverValue = orientation.z;
				break;
			case Driver::SCALE_X:
				driverValue = scale.x;
				break;
			case Driver::SCALE_Y:
				driverValue = scale.y;
				break;
			case Driver::SCALE_Z:
				driverValue = scale.z;
				break;
		}

		return driverValue;
	}

	void Node::attachChild(Node *child){
		child->setParent(this);
		children.push_back(child);
		child->updateAxis();
		child->onAttached();
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
		mesh->onAttached(this);
	}

	void Node::addSkeleton(Skeleton *skeleton){
			skeleton->onAttached(this);
			skeletons.push_back(skeleton);
	}

	void Node::attachParticleEmitter(ParticleEmitter *emitter){
		emitters.push_back(emitter);
		emitter->onAttached(this);
	}

	void Node::addLight(Light *light){
		Root::getSingleton()->shiftNumLights(true);
		lights.push_back(light);	
		light->onAttached(this);
		updateShaders();

	}

	void Node::removeLight(Light *light){
		for(int i = 0; i < lights.size(); i++)
			if(lights[i] == light){
				lights.erase(lights.begin() + i);

				Root::getSingleton()->shiftNumLights(false);
				updateShaders();

				break;
			}
	}

	void Node::removeLight(int id){
		Root::getSingleton()->shiftNumLights(false);
		Light *light = lights[id];
		lights.erase(lights.begin() + id);
		updateShaders();
	}

	void Node::addText(Text *text){
		texts.push_back(text);
		text->onAttached(this);	
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
		Vector3 xDir = orientation * Vector3(1, 0, 0),
					 	yDir = orientation * Vector3(0, 1, 0),
						zDir = orientation * Vector3(0, 0, 1);
		//newUp = Vector3(newUp.x, newUp.y, 0).norm();

		if(newUp != Vector3::VEC_ZERO){
			float angle = yDir.getAngleBetween(newUp);
			bool forw = (xDir.getAngleBetween(newUp) < PI / 2);
			setOrientation(Quaternion(angle * (forw ? -1 : 1), zDir) * orientation);
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
			parent ? parent->getGlobalAxis(0) : Vector3::VEC_I,
			parent ? parent->getGlobalAxis(1) : Vector3::VEC_J,
			parent ? parent->getGlobalAxis(2) : Vector3::VEC_K
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

	Node* Node::findDescendant(string name, bool allDescendants){
		vector<Node*> descendants = children;

		if(allDescendants){
			descendants.clear();
			getDescendants(descendants);
		}

		for(Node *desc : descendants)
			if(desc->getName() == name)
				return desc;

		return nullptr;
	}

	void Node::updateShaders(){
		Root *root = Root::getSingleton();
		AssetManager *am = AssetManager::getSingleton();
		ShaderAsset *sa = (ShaderAsset*)am->getAsset(root->getLibPath() + "texture.frag");
		string shaderStr = sa->shaderString;
		int numLights = root->getNumLights();

		string str1 = "const int numLights = " + to_string(numLights > 0 ? numLights : 1) + ";";
		int a1 = findNthOccurence(shaderStr, "\n", 0);
		int a2 = findNthOccurence(shaderStr, "\n", 1);
		shaderStr.replace(a1 + 1, a2 - a1 - 1, str1);

		string str2 = "const bool checkLights = " + string(numLights > 0 ? "true" : "false") + ";";
		a1 = findNthOccurence(shaderStr, "\n", 1);
		a2 = findNthOccurence(shaderStr, "\n", 2);
		shaderStr.replace(a1 + 1, a2 - a1 - 1, str2);

		ShaderAsset sa2(sa->path, shaderStr);
		am->editAsset(sa->path, sa2);
	}

	void Node::setOrientation(Quaternion q){
		this->orientation = q;
		updateAxis();
	}

	void Node::animate(float value, KeyframeChannel keyframeChannel){
		Vector3 newPos = pos, newScale = scale;
		Quaternion newRot = orientation;

		switch(keyframeChannel.type){
			case KeyframeChannel::POS_X:
				newPos.x = value;
				break;
			case KeyframeChannel::POS_Y:
				newPos.y = value;
				break;
			case KeyframeChannel::POS_Z:
				newPos.z = value;
				break;
			case KeyframeChannel::ROT_W:
				newRot.w = value;
				break;
			case KeyframeChannel::ROT_X:
				newRot.x = value;
				break;
			case KeyframeChannel::ROT_Y:
				newRot.y = value;
				break;
			case KeyframeChannel::ROT_Z:
				newRot.z = value;
				break;
			case KeyframeChannel::SCALE_X:
				newScale.x = value;
				break;
			case KeyframeChannel::SCALE_Y:
				newScale.y = value;
				break;
			case KeyframeChannel::SCALE_Z:
				newScale.z = value;
				break;
		}

		setPosition(newPos);
		setOrientation(newRot);
		setScale(newScale);
	}
}
