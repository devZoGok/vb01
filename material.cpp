#include "material.h"
#include "shader.h"
#include "root.h"

using namespace std;
using namespace glm;

namespace vb01{
	Material::Material(string shaderName, bool geometryShader){
		shader = new Shader(shaderName);
	}

	Material::~Material(){
		delete shader;
	}

	Material::Uniform* Material::getUniform(string name){
			Uniform *uni = nullptr;

			for(Uniform *u : uniforms)
					if(u->name == name){
							uni = u;
							break;
					}

			return uni;
	}

	void Material::update(){
		shader->use();

		int i = 0;

		for(Uniform *uni : uniforms){
				switch(uni->type){
						case Uniform::INT:{
								IntUniform *u = (IntUniform*)uni;
								shader->setInt(u->value, u->name);
								break;
						}
						case Uniform::BOOL:{
								BoolUniform *u = (BoolUniform*)uni;
								shader->setBool(u->value, u->name);
								break;
						}
						case Uniform::FLOAT:{
								FloatUniform *u = (FloatUniform*)uni;
								shader->setFloat(u->value, u->name);
								break;
						}
						case Uniform::VECTOR_2:{
								Vector2Uniform *u = (Vector2Uniform*)uni;
								shader->setVec2(u->value, u->name);
								break;
						}
						case Uniform::VECTOR_3:{
								Vector3Uniform *u = (Vector3Uniform*)uni;
								shader->setVec3(u->value, u->name);
								break;
						}
						case Uniform::VECTOR_4:{
								Vector4Uniform *u = (Vector4Uniform*)uni;
								shader->setVec4(u->value, u->name);
								break;
						}
						case Uniform::TEXTURE:{
								TextureUniform *u = (TextureUniform*)uni;
								shader->setInt(2 * i, u->name + (u->animatable ? ".pastTexture" : ""));
								shader->setBool(u->animatable, u->name + ".animated");

								if(u->value->getNumFrames() > 1){
									shader->setInt(2 * i + 1, u->name + ".nextTexture");
									shader->setFloat(u->value->getMixRatio(), u->name + ".mixRatio");
								}

								u->value->update(2 * i);

								++i;
								break;
						}
				}

		}
	}
}
