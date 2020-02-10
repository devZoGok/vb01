#ifndef NODE_H
#define NODE_H

#include"quaternion.h"
#include"vector.h"
#include<vector>

namespace vb01{
	class Mesh;
	class Model;
	class Light;
	class ParticleEmitter;
	class Text;

	class Node{
		public:
			struct Transform{
				Vector3 position,scale;
				Quaternion orientation;	
			};

			Node(Vector3=Vector3::VEC_ZERO,Quaternion=Quaternion::QUAT_W,Vector3=Vector3::VEC_IJK);
			virtual ~Node();
			void attachMesh(Mesh*);
			void attachParticleEmitter(ParticleEmitter*);
			virtual void update();
			void attachChild(Node*);
			void dettachChild(Node*);
			void addLight(Light*);
			void addText(Text*);
			void lookAt(Vector3,Vector3);
			Transform getWorldTransform();
			Vector3 getLocalAxis(int);
			Vector3 localToGlobal(Vector3);
			Vector3 globalToLocal(Vector3);
			inline Text* getText(int i){return texts[i];}
			inline Vector3 getWorldPosition(){return getWorldTransform().position;}
			inline Quaternion getWorldOrientation(){return getWorldTransform().orientation;}
			inline Vector3 getWorldScale(){return getWorldTransform().scale;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline Mesh* getMesh(int i){return meshes[i];}
			inline Node* getParent(){return parent;}
			inline void setParent(Node *par){this->parent=par;}
			inline Node* getChild(int i){return children[i];}
			inline Vector3 getPosition(){return pos;}
			inline Vector3 getScale(){return scale;}
			inline void setPosition(Vector3 pos){this->pos=pos;}
			inline void setScale(Vector3 scale){this->scale=scale;}
			inline std::vector<Node*>& getChildren(){return children;}
			inline std::vector<Light*>& getLights(){return lights;}
			inline Light* getLight(int i){return lights[i];}
			inline int getNumLights(){return lights.size();}
			inline int getNumChildren(){return children.size();}
			inline Quaternion getOrientation(){return orientation;}
			inline void setOrientation(Quaternion q){this->orientation=q;}
			inline bool isVisible(){return visible;}
			inline void setVisible(bool v){this->visible=v;}
			void getDescendants(Node*,std::vector<Node*>&);
		protected:
			bool visible=true;
			Vector3 pos,scale;
			Quaternion orientation;
			std::vector<Mesh*> meshes;
			std::vector<ParticleEmitter*> emitters;
			std::vector<Node*> children;
			std::vector<Light*> lights;
			std::vector<Text*> texts;
			Node *parent=nullptr;
	};
}

#endif
