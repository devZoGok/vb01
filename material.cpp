#include"material.h"
#include"shader.h"
#include"root.h"

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
			case MATERIAL_POST:
				shaderName="postProcess.";
				break;
			case MATERIAL_TEXT:
				shaderName="text.";
				break;
		}
		/*
		if(type==MATERIAL_2D)
			//shader=new Shader(basePath+shaderName+"vert",basePath+shaderName+"frag");
			shader=new Shader(basePath+shaderName+"vert",basePath+shaderName+"frag",basePath+shaderName+"geo");
		else
		*/
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
			shader->setBool(normalMapEnabled,"normalMapEnabled");
			shader->setInt(0,"diffuseMap");
			shader->setInt(1,"normalMap");
			shader->setInt(2,"specularMap");
			shader->setInt(3,"parallaxMap");
			shader->setInt(4,"environmentMap");
		}
		shader->setBool(texturingEnabled,"texturingEnabled");
		if(type==MATERIAL_GUI){
			shader->setBool(diffuseColorEnabled,"diffuseColorEnabled");
			if(diffuseColorEnabled)
				shader->setVec4(diffuseColor,"diffuseColor");
		}
		if(texturingEnabled){
			for(Texture *t : diffuseMapTextures)
				t->update(0);
			for(Texture *t : normalMapTextures)
				t->update(1);
			for(Texture *t : specularMapTextures)
				t->update(2);
			for(Texture *t : parallaxMapTextures)
				t->update(3);
		}
		else {
			shader->setVec4(diffuseColor,"diffuseColor");
		}
	}
}
