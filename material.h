#ifndef MATERIAL_H
#define MATERIAL_H

#include"texture.h"
#include"shader.h"

namespace vb01{
	class Material{
		public:
			Material();
			~Material();
			void update();
			void setDiffuseMap(std::string diffuseMap){this->diffuseMap=new Texture(diffuseMap);}
			void setNormalMap(std::string normalMap){this->normalMap=new Texture(normalMap);}
			void setSpecularMap(std::string specularMap){this->specularMap=new Texture(specularMap);}
			inline void setLightingEnabled(bool lighting){this->lightingEnabled=lighting;}
			inline Shader* getShader(){return shader;}
			inline bool isLighting(){return lightingEnabled;}
		private:
			bool lightingEnabled=false;
			Texture *diffuseMap=nullptr,*normalMap=nullptr,*specularMap=nullptr;
			Shader *shader;
	};
}

#endif
