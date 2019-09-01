#include"material.h"
#include"shader.h"

using namespace std;

namespace vb01{
	Material::Material(){
		string basePath="/home/dominykas/c++/FSim/";
		shader=new Shader(basePath+"texture.vert",basePath+"texture.frag");
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
