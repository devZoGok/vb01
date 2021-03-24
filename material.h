#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture.h"
#include "vector.h"
#include "animatable.h"

#include <vector>

namespace vb01{
	class Material : public Animatable{
		public:
			enum Type{MATERIAL_2D, MATERIAL_PARTICLE, MATERIAL_SKYBOX, MATERIAL_GUI, MATERIAL_POST, MATERIAL_TEXT};

			Material(Type = MATERIAL_2D);
			~Material();
			void update();
			inline void addDiffuseMap(Texture *texture){diffuseMapTextures.push_back(texture);}
			inline void addDiffuseMap(std::string diffuseMap[6]){diffuseMapTextures.push_back(new Texture(diffuseMap));}
			inline void addNormalMap(Texture *texture){normalMapTextures.push_back(texture);}
			inline void addSpecularMap(Texture *texture){specularMapTextures.push_back(texture);}
			inline void addParallaxMap(Texture *texture){parallaxMapTextures.push_back(texture);}
			inline void setDiffuseColor(Vector4 diffuse){this->diffuseColor = diffuse;}
			inline void setSpecularColor(Vector4 specular){this->specularColor = specular;}
			inline void setLightingEnabled(bool lighting){this->lightingEnabled = lighting;}
			inline void setTexturingEnabled(bool texturing){this->texturingEnabled = texturing;}
			inline void setDiffuseColorEnabled(bool diffuseColor){this->diffuseColorEnabled = diffuseColor;}
			inline void setNormalMapEnabled(bool enabled){this->normalMapEnabled = enabled;}
			inline void setParallaxMapEnabled(bool enabled){this->parallaxMapEnabled = enabled;}
			inline bool isLightingEnabled(){return lightingEnabled;}
			inline bool isTexturingEnabled(){return texturingEnabled;}
			inline bool isDiffuseColorEnabled(){return diffuseColorEnabled;}
			inline Texture* getDiffuseMap(int i){return diffuseMapTextures[i];}
			inline Texture* getNormalMap(int i){return normalMapTextures[i];}
			inline Texture* getSpecularMap(int i){return specularMapTextures[i];}
			inline Shader* getShader(){return shader;}
			inline Type getType(){return type;}
		private:
			Type type;
			bool lightingEnabled = false, texturingEnabled = true, diffuseColorEnabled = false, normalMapEnabled = false, parallaxMapEnabled = false;
			std::vector<Texture*> diffuseMapTextures, normalMapTextures, specularMapTextures, parallaxMapTextures;
			Vector4 diffuseColor = Vector4::VEC_IJKL, specularColor = Vector4::VEC_IJKL;
			Shader *shader = nullptr;

			void initShader();
			void animate(float, KeyframeChannel);
	};
}

#endif
