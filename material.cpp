#include"material.h"
#include"shader.h"

using namespace std;

namespace vb01{
	Material::Material(Type type){
		this->type=type;

		string basePath="../../vb01/",shaderName;
		switch(type){
			case MATERIAL_2D:
				shaderName="texture.";
				break;
			case MATERIAL_PARTICLE:
				shaderName="particle.";
				break;
			case MATERIAL_SKYBOX:
				shaderName="skybox.";
				break;
			case MATERIAL_GUI:
				shaderName="gui.";
				break;
			case MATERIAL_TEXT:
				shaderName="text.";
				break;
		}
		shader=new Shader(basePath+shaderName+"vert",basePath+shaderName+"frag");
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

	void Material::update(){
		shader->use();
		if(type==MATERIAL_2D){
			shader->setBool(lightingEnabled,"lightingEnabled");
			if(normalMapEnabled)
				shader->setBool(normalMapEnabled,"normalMapEnabled");
		}
		shader->setBool(texturingEnabled,"texturingEnabled");
		if(type==MATERIAL_GUI){
			shader->setBool(diffuseColorEnabled,"diffuseColorEnabled");
			if(diffuseColorEnabled)
				shader->setVec4(diffuseColor,"diffuseColor");
		}
		if(texturingEnabled){
			for(Texture *t : diffuseMapTextures)
				t->update();
			for(Texture *t : normalMapTextures)
				t->update(2);
			for(Texture *t : specularMapTextures)
				t->update(3);
		}
		else {
			shader->setVec4(diffuseColor,"diffuseColor");
		}
	}
}
