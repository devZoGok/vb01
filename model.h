#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include<string>
#include"node.h"

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;

namespace vb01{
	class Mesh;
	class Material;
	class Texture;

	class Model : public Node{
		public:
			Model(std::string);
			~Model();
			void update();
			void setMaterial(Material*);
		private:
			void processNode(aiNode*, const aiScene*, Node*);
			void processMesh(aiMesh*, const aiScene*, Node*);
//			std::vector<Texture*> processTexture(aiMaterial*, Assimp::aiTextureType);
			void processMaterial();

			Material* material;
	};
}

#endif