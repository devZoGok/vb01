#include"material.h"
#include"shader.h"

using namespace std;

namespace vb01{
	Material::Material(Type type){
		this->type=type;

		string basePath="/home/dominykas/c++/FSim/",shaderName;
		switch(type){
			case MATERIAL_2D:
				shaderName="texture.";
				break;
			case MATERIAL_SKYBOX:
				shaderName="skybox.";
				break;
		}
		shader=new Shader(basePath+shaderName+"vert",basePath+shaderName+"frag");
	}

	Material::~Material(){}

	void Material::update(){
		shader->use();
		shader->setBool(lightingEnabled,"lightingEnabled");
		for(Texture *t : diffuseMapTextures)
			t->select();
		for(Texture *t : normalMapTextures)
			t->select();
		for(Texture *t : specularMapTextures)
			t->select();
	}
}
