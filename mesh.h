#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "util.h"
#include "material.h"
#include "attachable.h"
#include "meshData.h"

#include <vector>
#include <string>

namespace vb01{
	class Node;
	class Shader;
	class Texture;
	class Skeleton;

	class Mesh : public Attachable{
		public:
			Mesh(MeshData);
			~Mesh();
			void updateVerts(MeshData);
			virtual void update();
			void render();
			void setReflect(bool);
			inline void setMaterial(Material *mat){this->material = mat;}
			inline void setCastShadow(bool castShadow){this->castShadow = castShadow;}
			inline void setReflective(bool r){this->reflective = r;}
			inline void setWireframe(bool w){this->wireframe = w;}
			inline void setSkeleton(Skeleton *sk){this->skeleton = sk;}
			inline Material* getMaterial(){return material;}
			inline bool isReflective(){return reflective;}
			inline bool isCastShadow(){return castShadow;}
			inline MeshData::ShapeKey* getShapeKey(int i){return &shapeKeys[i];}
			inline Skeleton* getSkeleton(){return skeleton;}
			inline Texture* getPrefilterMap(){return prefilterMap;}
			inline Texture* getIrradianceMap(){return irradianceMap;}
			inline Texture* getPostfilterMap(){return postfilterMap;}
			inline Texture* getBrdfIntegrationMap(){return brdfIntegrationMap;}
			inline MeshData getMeshBase(){return meshBase;}
		private:
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
			std::vector<Mesh*> meshes;
			Skeleton *skeleton = nullptr;
		protected:
			Mesh(){}

			MeshData meshBase;
			int numShapeKeys = 0;
			MeshData::ShapeKey *shapeKeys = nullptr;
			bool castShadow = false, reflect = false, reflective = false, wireframe = false;
	};
}

#endif
