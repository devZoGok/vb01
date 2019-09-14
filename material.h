#ifndef MATERIAL_H
#define MATERIAL_H

#include"shader.h"
#include"texture.h"
#include<vector>

namespace vb01{
	class Material{
		public:
			enum Type{MATERIAL_2D,MATERIAL_PARTICLE,MATERIAL_SKYBOX};

			Material(Type=MATERIAL_2D);
			~Material();
			void update();
			void addDiffuseMap(std::string diffuseMap){diffuseMapTextures.push_back(new Texture(diffuseMap));}
			void addDiffuseMap(std::string diffuseMap[6]){diffuseMapTextures.push_back(new Texture(diffuseMap));}
			void addNormalMap(std::string normalMap){normalMapTextures.push_back(new Texture(normalMap));}
			void addSpecularMap(std::string specularMap){specularMapTextures.push_back(new Texture(specularMap));}
			inline void setLightingEnabled(bool lighting){this->lightingEnabled=lighting;}
			inline Shader* getShader(){return shader;}
			inline bool isLightingEnabled(){return lightingEnabled;}
		private:
			Type type;
			bool lightingEnabled=false;
			std::vector<Texture*> diffuseMapTextures,normalMapTextures,specularMapTextures;
			Shader *shader=nullptr;
	};
}

#endif
