#ifndef MATERIAL_H
#define MATERIAL_H

#include"shader.h"
#include"texture.h"
#include"vector.h"
#include<vector>

namespace vb01{
	class Material{
		public:
			enum Type{MATERIAL_2D,MATERIAL_PARTICLE,MATERIAL_SKYBOX,MATERIAL_GUI,MATERIAL_TEXT};

			Material(Type=MATERIAL_2D);
			~Material();
			void update();
			void addDiffuseMap(std::string diffuseMap){diffuseMapTextures.push_back(new Texture(diffuseMap));}
			void addDiffuseMap(std::string diffuseMap[6]){diffuseMapTextures.push_back(new Texture(diffuseMap));}
			void addDiffuseMap(Texture *texture){diffuseMapTextures.push_back(texture);}
			void addNormalMap(std::string normalMap){normalMapTextures.push_back(new Texture(normalMap));}
			void addSpecularMap(std::string specularMap){specularMapTextures.push_back(new Texture(specularMap));}
			inline void setDiffuseColor(Vector4 diffuse){this->diffuseColor=diffuse;}
			inline void setSpecularColor(Vector4 specular){this->specularColor=specular;}
			inline void setLightingEnabled(bool lighting){this->lightingEnabled=lighting;}
			inline void setTexturingEnabled(bool texturing){this->texturingEnabled=texturing;}
			inline void setDiffuseColorEnabled(bool diffuseColor){this->diffuseColorEnabled=diffuseColor;}
			inline bool isLightingEnabled(){return lightingEnabled;}
			inline bool isTexturingEnabled(){return texturingEnabled;}
			inline bool isDiffuseColorEnabled(){return diffuseColorEnabled;}
			inline Shader* getShader(){return shader;}
			inline Texture* getDiffuseMap(int i){return diffuseMapTextures[i];}
			inline Type getType(){return type;}
		private:
			Type type;
			bool lightingEnabled=false,texturingEnabled=true,diffuseColorEnabled=false;
			std::vector<Texture*> diffuseMapTextures,normalMapTextures,specularMapTextures;
			Vector4 diffuseColor=Vector4::VEC_IJKL,specularColor=Vector4::VEC_IJKL;
			Shader *shader=nullptr;
	};
}

#endif
