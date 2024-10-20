#ifndef NODE_H
#define NODE_H

#include "quaternion.h"
#include "vector.h"
#include "root.h"
#include "animatable.h"
#include "driver.h"

#include <vector>
#include <string>

namespace vb01{
	class Model;
	class Light;
	class ParticleEmitter;
	class Text;
	class Skeleton;
	class AnimationController;

	class Node : public Animatable{
		public:
			Node(Vector3 = Vector3::VEC_ZERO, Quaternion = Quaternion::QUAT_W, Vector3 = Vector3::VEC_IJK, std::string = "", Animatable::Type = Animatable::NODE);
			virtual ~Node();
			Node* clone();
			void addSkeleton(Skeleton*);
			void attachMesh(Mesh*);
			void attachParticleEmitter(ParticleEmitter*);
			virtual void update();
			virtual void onAttached(){}
			void attachChild(Node*);
			void dettachChild(Node*);
			void addLight(Light*);
			void removeLight(Light*);
			void removeLight(int);
			void addText(Text*);
			virtual void lookAt(Vector3, Vector3);
			virtual void lookAt(Vector3);
			void updateAxis();
			void setOrientation(Quaternion);
			void getDescendants(std::vector<Node*>&);
			std::vector<Node*> getAncestors(Node *anc = Root::getSingleton()->getRootNode());
			Vector3 localToGlobalPosition(Vector3);
			Vector3 globalToLocalPosition(Vector3);
			Quaternion localToGlobalOrientation(Quaternion);
			Quaternion globalToLocalOrientation(Quaternion);
			Vector3 localToGlobalScale(Vector3);
			Vector3 globalToLocalScale(Vector3);
			Node* findDescendant(std::string, bool=false);
			inline void dettachMesh(int id){meshes.erase(meshes.begin() + id);}
			inline Skeleton* getSkeleton(int i){return skeletons[i];}
			inline int getNumSkeletons(){return skeletons.size();}
			inline Text* getText(int i){return texts[i];}
			inline std::vector<Text*>& getTexts(){return texts;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline std::vector<Skeleton*>& getSkeletons(){return skeletons;}
			inline Mesh* getMesh(int i){return meshes[i];}
			inline int getNumMeshes(){return meshes.size();}
			inline ParticleEmitter* getParticleEmitter(int i){return emitters[i];}
			inline Node* getParent(){return parent;}
			inline void setParent(Node *par){this->parent = par;}
			inline Node* getChild(int i){return children[i];}
			inline Vector3 getGlobalAxis(int i){return globalAxis[i];}
			inline Vector3 getPosition(){return pos;}
			inline Vector3 getScale(){return scale;}
			inline void setPosition(Vector3 pos){this->pos = pos;}
			inline void setScale(Vector3 scale){this->scale = scale;}
			inline std::vector<Node*>& getChildren(){return children;}
			inline std::vector<Light*>& getLights(){return lights;}
			inline Light* getLight(int i){return lights[i];}
			inline int getNumLights(){return lights.size();}
			inline int getNumChildren(){return children.size();}
			inline Quaternion getOrientation(){return orientation;}
			inline bool isVisible(){return visible;}
			inline void setVisible(bool v){this->visible = v;}
			inline void addDriver(Driver *d){drivers.push_back(d);}
			inline Driver* getDriver(int i){return drivers[i];}
			inline std::vector<Driver*> getDrivers(){return drivers;}
		private:
			void adjustUp(Vector3);
			void adjustDir(Vector3);
			void updateShaders();

			Quaternion adjustRot(std::vector<Node*>, Quaternion, bool);
			std::vector<Driver*> drivers;
		protected:
			virtual void animate(float, KeyframeChannel);
			virtual float getDriverValue(Driver::VariableType);
			
			bool visible = true;
			Vector3 pos, scale, globalAxis[3]{Vector3::VEC_I, Vector3::VEC_J, Vector3::VEC_K};
			Quaternion orientation;
			std::vector<Mesh*> meshes;
			std::vector<ParticleEmitter*> emitters;
			std::vector<Node*> children;
			std::vector<Light*> lights;
			std::vector<Text*> texts;
			std::vector<Skeleton*> skeletons;
			Node *parent = nullptr;
	};
}

#endif
