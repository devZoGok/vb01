#ifndef MESH_H
#define MESH_H

#include"vector.h"
#include<vector>
#include<string>
#include"material.h"


namespace vb01{
	class Node;
	class Skeleton;

	class Mesh{
		public:
			struct Vertex{
				Vector3 pos,norm;
				Vector2 texCoords;	
			};
			struct VertexGroup{
				int numVertices=0;
				Vertex **vertices=nullptr;
				float *weights;
				std::string name;
			};

			Mesh(Vertex*,unsigned int*,int);
			~Mesh();
			virtual void update();
			void render();
			void setMaterial(Material *mat){this->material=mat;}
			inline void setNode(Node *node){this->node=node;}
			inline void setCastShadow(bool castShadow){this->castShadow=castShadow;}
			inline Node* getNode(){return node;}
			inline Material* getMaterial(){return material;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline bool isCastShadow(){return castShadow;}
			inline int getNumVerts(){return 3*numTris;}
			inline Vertex* getVerts(){return vertices;}
			inline unsigned int* getIndices(){return indices;}
		private:
			Material *material=nullptr;
			Node *node=nullptr;
			std::vector<Mesh*> meshes;
			std::string name="";
			Skeleton *skeleton=nullptr;
		protected:
			Mesh();
			void construct();

			bool staticVerts=true,castShadow=false;
			Vertex *vertices;
			VertexGroup groups;
			unsigned int *indices,VAO,VBO,EBO;
			int numTris;
	};
}

#endif
