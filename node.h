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
			Node(Vector3=Vector3::VEC_ZERO);
			~Node();
			void attachMesh(Mesh*);
			void attachParticleEmitter(ParticleEmitter*);
			virtual void update();
			void attachChild(Node*);
			void addLight(Light*);
			void addText(Text*);
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline Node* getParent(){return parent;}
			inline Vector3 getPosition(){return pos;}
			inline std::vector<Node*>& getDescendants(){return descendants;}
			inline std::vector<Node*>& getChildren(){return children;}
			inline std::vector<Light*>& getLights(){return lights;}
			inline Light* getLight(int i){return lights[i];}
			inline int getNumLights(){return lights.size();}
		protected:
			Vector3 pos;
			std::vector<Mesh*> meshes;
			std::vector<ParticleEmitter*> emitters;
			std::vector<Node*> children,descendants;
			std::vector<Light*> lights;
			std::vector<Text*> texts;
			Node *parent=nullptr;
	};
}

#endif
