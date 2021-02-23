#ifndef NODE_H
#define NODE_H

#include "quaternion.h"
#include "vector.h"
#include "root.h"
#include "animatable.h"

#include <vector>
#include <string>

namespace vb01{
	class Mesh;
	class Model;
	class Light;
	class ParticleEmitter;
	class Text;
	class AnimationController;

	class Node : public Animatable{
		public:
			struct Transform{
				Vector3 position = Vector3::VEC_ZERO, scale = Vector3::VEC_IJK;
				Quaternion orientation = Quaternion::QUAT_W;	
			};

			Node(Vector3 = Vector3::VEC_ZERO, Quaternion = Quaternion::QUAT_W, Vector3 = Vector3::VEC_IJK, std::string = "", AnimationController *c = nullptr);
			virtual ~Node();
			void attachMesh(Mesh*);
			void attachParticleEmitter(ParticleEmitter*);
			virtual void update();
			void attachChild(Node*);
			void dettachChild(Node*);
			void addLight(Light*);
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
			inline Text* getText(int i){return texts[i];}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline Mesh* getMesh(int i){return meshes[i];}
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
			inline std::string getName(){return name;}
			inline void setVisible(bool v){this->visible = v;}
		private:
			void adjustUp(Vector3);
			void adjustDir(Vector3);
			void updateShaders();

			Quaternion adjustRot(std::vector<Node*>, Quaternion, bool);
			AnimationController *controller = nullptr;
		protected:
			virtual void animate(float, KeyframeChannel);
			
			bool visible = true;
			Vector3 pos, scale, globalAxis[3];
			Quaternion orientation;
			std::vector<Mesh*> meshes;
			std::vector<ParticleEmitter*> emitters;
			std::vector<Node*> children;
			std::vector<Light*> lights;
			std::vector<Text*> texts;
			std::string name;
			Node *parent = nullptr;
	};
}

#endif
