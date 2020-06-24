#ifndef NODE_H
#define NODE_H

#include"quaternion.h"
#include"vector.h"
#include<vector>
#include<string>

namespace vb01{
	class Mesh;
	class Model;
	class Light;
	class ParticleEmitter;
	class Text;

	class Node{
		public:
			struct Transform{
				Vector3 position=Vector3::VEC_ZERO,scale=Vector3::VEC_IJK;
				Quaternion orientation=Quaternion::QUAT_W;	
			};

			Node(Vector3=Vector3::VEC_ZERO,Quaternion=Quaternion::QUAT_W,Vector3=Vector3::VEC_IJK,std::string="");
			virtual ~Node();
			void attachMesh(Mesh*);
			void attachParticleEmitter(ParticleEmitter*);
			virtual void update();
			void attachChild(Node*);
			void dettachChild(Node*);
			void addLight(Light*);
			void removeLight(int);
			void addText(Text*);
			void lookAt(Vector3,Vector3,Node*);
			void updateLocalAxis();
			void setOrientation(Quaternion);
			void getDescendants(Node*,std::vector<Node*>&);
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
			inline void setParent(Node *par){this->parent=par;}
			inline Node* getChild(int i){return children[i];}
			inline Vector3 getLocalAxis(int i){return axis[i];}
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
			inline bool isVisible(){return visible;}
			inline std::string getName(){return name;}
			inline void setVisible(bool v){this->visible=v;}
		protected:
			void updateShaders();
			
			bool visible=true;
			Vector3 pos,scale,axis[3];
			Quaternion orientation;
			std::vector<Mesh*> meshes;
			std::vector<ParticleEmitter*> emitters;
			std::vector<Node*> children;
			std::vector<Light*> lights;
			std::vector<Text*> texts;
			std::string name;
			Node *parent=nullptr;
	};
}

#endif
