#ifndef MESH_H
#define MESH_H

#include"vector.h"
#include"util.h"
#include<vector>
#include<string>
#include"material.h"


namespace vb01{
	class Node;
	class Shader;
	class Texture;
	class Skeleton;

	class Mesh{
		public:
			struct Vertex{
				Vector3 pos,tan,biTan,norm;
				Vector2 uv;	
			};
			struct VertexGroup{
				int numVertices;
				u32 *vertices=nullptr;
				float *weights=nullptr;
				std::string name;
			};
			struct ShapeKey{
				int numVertices;
				Vertex **vertices=nullptr;
				Vector3 *offsets=nullptr;
				std::string name;
			};

			Mesh(Vertex*,unsigned int*,int,VertexGroup *groups=nullptr,int=0,ShapeKey *keys=nullptr,int=0,std::string="");
			~Mesh();
			virtual void update();
			void render();
			void setMaterial(Material *mat){this->material=mat;}
			VertexGroup& getVertexGroup(std::string);
			ShapeKey& getShapeKey(std::string);
			inline void setNode(Node *node){this->node=node;}
			inline void setCastShadow(bool castShadow){this->castShadow=castShadow;}
			inline void setReflect(bool r){this->reflect=r;}
			inline void setWireframe(bool w){this->wireframe=w;}
			inline void setSkeleton(Skeleton *sk){this->skeleton=sk;}
			inline Node* getNode(){return node;}
			inline Material* getMaterial(){return material;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline bool isCastShadow(){return castShadow;}
			inline int getNumVerts(){return 3*numTris;}
			inline Vertex* getVerts(){return vertices;}
			inline std::string getName(){return name;}
			inline Skeleton* getSkeleton(){return skeleton;}
			inline unsigned int* getIndices(){return indices;}
		private:
			Shader *environmentShader=nullptr;
			u32 environmentBuffer;
			Texture *environmentMap;
			Material *material=nullptr;
			Node *node=nullptr;
			std::vector<Mesh*> meshes;
			std::string name="";
			int width=800;
			Skeleton *skeleton=nullptr;
		protected:
			Mesh();
			void construct();

			bool staticVerts=true,castShadow=false,reflect=false,wireframe=false;
			Vertex *vertices;
			VertexGroup *groups;
			ShapeKey *shapeKeys;
			unsigned int *indices,VAO,VBO,EBO;
			int numTris,numVertexGroups=0,numShapeKeys=0;
	};
}

#endif
