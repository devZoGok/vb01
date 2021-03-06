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

	void Material::animate(float value, KeyframeChannel keyframeChannel){
		switch(keyframeChannel.type){
			case KeyframeChannel::DIFFUSE_COLOR_W:
				diffuseColor.w = value;
				break;
			case KeyframeChannel::DIFFUSE_COLOR_X:
				diffuseColor.x = value;
				break;
			case KeyframeChannel::DIFFUSE_COLOR_Y:
				diffuseColor.y = value;
				break;
			case KeyframeChannel::DIFFUSE_COLOR_Z:
				diffuseColor.z = value;
				break;
			case KeyframeChannel::SPECULAR_COLOR_W:
				specularColor.w = value;
				break;
			case KeyframeChannel::SPECULAR_COLOR_X:
				specularColor.x = value;
				break;
			case KeyframeChannel::SPECULAR_COLOR_Y:
				specularColor.y = value;
				break;
			case KeyframeChannel::SPECULAR_COLOR_Z:
				specularColor.z = value;
				break;
		}
	}

	void Material::update(){
		shader->use();
		shader->setBool(texturingEnabled, "texturingEnabled");
		const int DIFFUSE_SLOT = Texture::DIFFUSE * 2;
		const int NORMAL_SLOT = Texture::NORMAL * 2;
		const int SPECULAR_SLOT = Texture::SPECULAR * 2;
		const int PARALLAX_SLOT = Texture::PARALLAX * 2;
		const int ENVIRONMENT_SLOT = Texture::ENVIRONMENT * 2;

		if(type == MATERIAL_2D){
			shader->setBool(lightingEnabled, "lightingEnabled");
			shader->setBool(normalMapEnabled, "normalMapEnabled");
			shader->setInt(DIFFUSE_SLOT, "textures["+to_string(DIFFUSE_SLOT)+"].pastTexture");
			shader->setInt(NORMAL_SLOT, "textures["+to_string(NORMAL_SLOT)+"].pastTexture");
			shader->setInt(SPECULAR_SLOT, "textures["+to_string(SPECULAR_SLOT)+"].pastTexture");
			shader->setInt(PARALLAX_SLOT, "textures["+to_string(PARALLAX_SLOT)+"].pastTexture");
			shader->setInt(ENVIRONMENT_SLOT, "environmentMap");
		}
		else if(type == MATERIAL_GUI){
			shader->setBool(diffuseColorEnabled, "diffuseColorEnabled");
			if(diffuseColorEnabled)
				shader->setVec4(diffuseColor, "diffuseColor");
		}

		if(texturingEnabled){
			int textureSlot = DIFFUSE_SLOT; 
			for(Texture *t : diffuseMapTextures){
				t->update(textureSlot);
				if(t->getNumFrames() > 0)
					shader->setBool(true, "textures[" + to_string(textureSlot) + "].animated");
			}
			for(Texture *t : normalMapTextures)
				t->update(NORMAL_SLOT);
			for(Texture *t : specularMapTextures)
				t->update(SPECULAR_SLOT);
			for(Texture *t : parallaxMapTextures)
				t->update(PARALLAX_SLOT);
		}
		else 
			shader->setVec4(diffuseColor, "diffuseColor");
	}
}
