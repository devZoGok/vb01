#include"material.h"
#include"shader.h"
#include"root.h"

using namespace std;

namespace vb01{
	Material::Material(Type type){
		this->type=type;

		initShader();
	}

	Material::~Material(){
		delete shader;
		for(Texture *t : diffuseMapTextures)
			delete t;
		for(Texture *t : normalMapTextures)
			delete t;
		for(Texture *t : specularMapTextures)
			delete t;
	}

	void Material::initShader(){
		string basePath = "../../vb01/", shaderName;
		switch(type){
			case MATERIAL_2D:
				shaderName = "texture.";
				break;
			case MATERIAL_PARTICLE:
				shaderName = "particle.";
				break;
			case MATERIAL_SKYBOX:
				shaderName = "skybox.";
				break;
			case MATERIAL_GUI:
				shaderName = "gui.";
				break;
			case MATERIAL_POST:
				shaderName = "postProcess.";
				break;
			case MATERIAL_TEXT:
				shaderName = "text.";
				break;
		}
		/*
		if(type==MATERIAL_2D)
			//shader=new Shader(basePath+shaderName+"vert",basePath+shaderName+"frag");
			shader=new Shader(basePath+shaderName+"vert",basePath+shaderName+"frag",basePath+shaderName+"geo");
		else
		*/
			shader = new Shader(basePath + shaderName + "vert", basePath + shaderName + "frag");
	}

	void Material::update(){
		shader->use();
		shader->setBool(texturingEnabled, "texturingEnabled");
		const int TEXTURE_SLOTS[]{
			Texture::DIFFUSE * 2,
			Texture::NORMAL * 2,
			Texture::SPECULAR * 2,
			Texture::PARALLAX * 2,
			Texture::ENVIRONMENT * 2
		};

		if(type == MATERIAL_2D){
			shader->setBool(lightingEnabled, "lightingEnabled");
			shader->setBool(normalMapEnabled, "normalMapEnabled");
			for(int i = 0; i < 4; i++){
				shader->setInt(TEXTURE_SLOTS[i], "textures[" + to_string(TEXTURE_SLOTS[i]) + "].pastTexture");
				shader->setInt(TEXTURE_SLOTS[i] + 1, "textures[" + to_string(TEXTURE_SLOTS[i]) + "].nextTexture");
			}
			shader->setInt(TEXTURE_SLOTS[4], "environmentMap");
		}
		else if(type == MATERIAL_GUI){
			shader->setBool(diffuseColorEnabled, "diffuseColorEnabled");
			if(diffuseColorEnabled)
				shader->setVec4(diffuseColor, "diffuseColor");
		}

		if(texturingEnabled){
			vector<Texture*> textures;
			textures.insert(textures.end(), diffuseMapTextures.begin(), diffuseMapTextures.end());
			textures.insert(textures.end(), normalMapTextures.begin(), normalMapTextures.end());
			textures.insert(textures.end(), specularMapTextures.begin(), specularMapTextures.end());
			textures.insert(textures.end(), parallaxMapTextures.begin(), parallaxMapTextures.end());

			for(Texture *t : textures){
				int id = t->getTextureTypeId() * 2;
				shader->setBool(t->getNumFrames() > 0, "textures[" + to_string(id) + "].animated");
				shader->setFloat(t->getMixRatio(), "textures[" + to_string(id) + "].mixRatio");
				t->update(id);
			}
		}
		else 
			shader->setVec4(diffuseColor, "diffuseColor");
	}
}
