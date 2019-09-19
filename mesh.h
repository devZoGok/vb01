#ifndef MESH_H
#define MESH_H

#include"vector.h"
#include<vector>
#include<string>
#include"material.h"


namespace vb01{
	class Node;

	class Mesh{
		public:
			struct Vertex{
				Vector3 pos,norm;
				Vector2 texCoords;	
			};
			Mesh(Vertex*,unsigned int*,int);
			~Mesh();
			virtual void update();
			void setMaterial(Material *mat){this->material=mat;}
			inline void setNode(Node *node){this->node=node;}
			inline Node* getNode(){return node;}
			inline Material* getMaterial(){return material;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
		private:
			Material *material=nullptr;
			Node *node=nullptr;
			std::vector<Mesh*> meshes;
			std::string name="";
		protected:
			Mesh();
			void construct();

			bool staticVerts=true;
			Vertex *vertices;
			unsigned int *indices,VAO,VBO,EBO;
			int numTris;
	};
}

#endif
