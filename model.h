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
			Model(std::string,bool=true);
			~Model();
			void update();
			void setMaterial(Material*);
			void setCastShadow(bool);
			void setReflect(bool);
			void setWireframe(bool);
		private:
			void processNode(aiNode*, const aiScene*, Node*);
			void processMesh(aiMesh*, const aiScene*, Node*);
//			std::vector<Texture*> processTexture(aiMaterial*, Assimp::aiTextureType);
			void processMaterial();
			void getLineData(std::string&,std::string[],int,int=0);

			Material* material;
			bool castShadow=false,reflect=false,wireframe=false;
	};
}

#endif
