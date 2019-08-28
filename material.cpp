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
		if(diffuseMap)diffuseMap->select();
		if(normalMap)normalMap->select();
		if(specularMap)specularMap->select();
	}
}
