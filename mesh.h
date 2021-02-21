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
				Vector3 pos, norm, tan, biTan;
				Vector2 uv;	
				float weights[4]{0, 0, 0, 0};
				int boneIndices[4]{-1, -1, -1, -1};
				Vector3 shapeKeyOffsets[100];
			};

			Mesh(Vertex*, unsigned int*, int, std::string *vg = nullptr, int = 0, std::string *sk = nullptr, int = 0, std::string = "");
			~Mesh();
			void construct();
			virtual void update();
			void render();
			void setMaterial(Material *mat){this->material = mat;}
			inline void setNode(Node *node){this->node = node;}
			inline void setCastShadow(bool castShadow){this->castShadow = castShadow;}
			inline void setReflect(bool r){this->reflect = r;}
			inline void setWireframe(bool w){this->wireframe = w;}
			inline void setSkeleton(Skeleton *sk){this->skeleton = sk;}
			inline Node* getNode(){return node;}
			inline Material* getMaterial(){return material;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline bool isCastShadow(){return castShadow;}
			inline int getNumVerts(){return 3 * numTris;}
			inline Vertex* getVerts(){return vertices;}
			inline std::string getName(){return name;}
			inline Skeleton* getSkeleton(){return skeleton;}
			inline std::string* getVertexGroups(){return vertexGroups;}
			inline int getNumVertexGroups(){return numVertexGroups;}
			inline unsigned int* getIndices(){return indices;}
		private:
			void initMesh();
			void initFramebuffer();
			void updateSkeleton(Shader*);
			void updateReflection(Shader*, Vector3, int, int);
			void updateShapeKeys(Shader*);

			Shader *environmentShader = nullptr;
			u32 environmentBuffer;
			Texture *environmentMap;
			Material *material = nullptr;
			Node *node = nullptr;
			std::vector<Mesh*> meshes;
			std::string name = "";
			Skeleton *skeleton = nullptr;
		protected:
			Mesh();

			bool staticVerts = true, castShadow = false, reflect = false, wireframe = false;
			Vertex *vertices;
			std::string *vertexGroups = nullptr, *shapeKeys = nullptr;
			u32 *indices, VAO, VBO, EBO;
			float *shapeKeyFactors = nullptr;
			int numTris, numVertexGroups = 0, numShapeKeys = 0;
	};
}

#endif
