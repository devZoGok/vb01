#ifndef MESH_H
#define MESH_H

#include"vector.h"
#include<vector>
#include<string>
#include"material.h"

namespace vb01{
	class Node;

	class Mesh{
		protected:
			struct Vertex{
				Vector3 pos,norm;
				Vector2 texCoords;	
			};
			void construct();
			Vertex *vertices;
			unsigned int *indices,VAO,VBO,EBO;
			int numTris;
			Node *node=nullptr;
		public:
			Mesh(std::string);
			Mesh();
			~Mesh();
			virtual void update();
			void setMaterial(Material *mat){this->material=mat;}
			inline void setNode(Node *node){this->node=node;}
			inline Node* getNode(){return node;}
		private:
			Material *material=nullptr;
	};
}

#endif
