#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "util.h"
#include "material.h"
#include "animatable.h"
#include <vector>
#include <string>

namespace vb01{
	class Node;
	class Shader;
	class Texture;
	class Skeleton;

	class Mesh : public Animatable{
		public:
			struct ShapeKey : public Animatable{
				std::string name;
				float minValue, value, maxValue;

				void animate(float, KeyframeChannel);
			};

			struct Vertex{
				Vector3 pos, norm, tan, biTan;
				Vector2 uv;	
				float weights[4]{0, 0, 0, 0};
				int boneIndices[4]{-1, -1, -1, -1};
				Vector3 shapeKeyOffsets[100];
			};

			Mesh(Vertex*, u32*, int, std::string *vg = nullptr, int = 0, ShapeKey *sk = nullptr, int = 0, std::string = "");
			~Mesh();
			void construct();
			virtual void update();
			void render();
			void setMaterial(Material *mat){this->material = mat;}
			inline void setNode(Node *node){this->node = node;}
			inline void setCastShadow(bool castShadow){this->castShadow = castShadow;}
			inline void setReflect(bool r){this->reflect = r;}
			inline void setReflective(bool r){this->reflective = r;}
			inline void setWireframe(bool w){this->wireframe = w;}
			inline void setSkeleton(Skeleton *sk){this->skeleton = sk;}
			inline Node* getNode(){return node;}
			inline Material* getMaterial(){return material;}
			inline std::vector<Mesh*>& getMeshes(){return meshes;}
			inline bool isReflective(){return reflective;}
			inline bool isCastShadow(){return castShadow;}
			inline int getNumVerts(){return 3 * numTris;}
			inline Vertex* getVerts(){return vertices;}
			inline std::string getName(){return name;}
			inline Skeleton* getSkeleton(){return skeleton;}
			inline std::string* getVertexGroups(){return vertexGroups;}
			inline int getNumVertexGroups(){return numVertexGroups;}
			inline u32* getIndices(){return indices;}
			inline ShapeKey& getShapeKey(int i){return shapeKeys[i];}
			inline int getNumShapeKeys(){return numShapeKeys;}
			inline Texture* getPrefilterMap(){return prefilterMap;}
			inline Texture* getIrradianceMap(){return irradianceMap;}
			inline Texture* getPostfilterMap(){return postfilterMap;}
			inline Texture* getBrdfIntegrationMap(){return brdfIntegrationMap;}
		private:
			void initMesh();
			void initFramebuffer(u32&, u32&, int);
			void updateSkeleton(Shader*);
			void updatePrefilterMap(Vector3, glm::mat4&, glm::vec3[6]);
			void updateIrradianceMap(Vector3, glm::mat4&, glm::vec3[6]);
			void updatePostfilterMap(glm::mat4&, glm::vec3[6]);
			void updateBrdfLut();
			void updateReflection(Vector3);
			void updateShapeKeys(Shader*);
			inline glm::vec3 getUpVec(glm::vec3 dir){return (fabs(dir.y) == 1 ? glm::vec3(0, 0, -1) : glm::vec3(0, 1, 0));}

			int irradianceMapSize = 32, preFilterMapSize = 512, environmentMapSize = 128, brdfMapSize = 512;
			u32 numSamples = 1024; 
			float irradianceSampleDelta = 0.5;
			Texture *prefilterMap = nullptr, *irradianceMap = nullptr, *postfilterMap = nullptr, *brdfIntegrationMap = nullptr;
			Shader *environmentShader = nullptr, *irradianceShader = nullptr, *brdfIntegrationShader = nullptr;
			u32 preFilterFramebuffer, preFilterRenderbuffer, irrandianceFramebuffer, irradianceRenderbuffer, postFilterFramebuffer, postFilterRenderbuffer, brdfFramebuffer, brdfRenderbuffer;
			Material *material = nullptr;
			Node *node = nullptr;
			std::vector<Mesh*> meshes;
			std::string name = "";
			Skeleton *skeleton = nullptr;
		protected:
			Mesh();

			bool staticVerts = true, castShadow = false, reflect = false, reflective = false, wireframe = false;
			Vertex *vertices;
			std::string *vertexGroups = nullptr;
			ShapeKey *shapeKeys = nullptr;
			u32 *indices, VAO, VBO, EBO;
			int numTris, numVertexGroups = 0, numShapeKeys = 0;
	};
}

#endif
