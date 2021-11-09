#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture.h"
#include "vector.h"
#include "animatable.h"

#include <vector>
#include <map>

namespace vb01{
		class Texture;

	class Material : public Animatable{
		public:
			struct Uniform : public Animatable{
					void animate(float val, KeyframeChannel channel){
							switch(channel.type){
									case KeyframeChannel::UNIFORM_1:
											animateComponent1(val);
											break;
									case KeyframeChannel::UNIFORM_2:
											animateComponent2(val);
											break;
									case KeyframeChannel::UNIFORM_3:
											animateComponent3(val);
											break;
									case KeyframeChannel::UNIFORM_4:
											animateComponent4(val);
											break;
							}
					}

					enum Type{
							INT,
						 	BOOL,
						 	FLOAT,
						 	VECTOR_2,
						 	VECTOR_3,
						 	VECTOR_4,
						 	TEXTURE
					};

					std::string name;
					Type type;

					protected:
					virtual void animateComponent1(float val){}
					virtual void animateComponent2(float val){}
					virtual void animateComponent3(float val){}
					virtual void animateComponent4(float val){}
			};

			struct IntUniform : public Uniform{
					int value;

					IntUniform(std::string name, int value){
							this->name = name;
							this->value = value;
							this->type = Uniform::INT;
					}

					void animateComponent1(float val){value = (int)val;}
			};

			struct BoolUniform : public Uniform{
					bool value;

					BoolUniform(std::string name, bool value){
							this->name = name;
							this->value = value;
							this->type = Uniform::BOOL;
					}

					void animateComponent1(float val){value = (bool)val;}
			};

			struct FloatUniform : public Uniform{
					float value;

					FloatUniform(std::string name, float value){
							this->name = name;
							this->value = value;
							this->type = Uniform::FLOAT;
					}

					void animateComponent1(float val){value = val;}
			};

			struct Vector2Uniform : public Uniform{
					Vector2 value;

					Vector2Uniform(std::string name, Vector2 value){
							this->name = name;
							this->value = value;
							this->type = Uniform::VECTOR_2;
					}

					void animateComponent1(float val){value.x = val;}

					void animateComponent2(float val){value.y = val;}
			};

			struct Vector3Uniform : public Uniform{
					Vector3 value;

					Vector3Uniform(std::string name, Vector3 value){
							this->name = name;
							this->value = value;
							this->type = Uniform::VECTOR_3;
					}

					void animateComponent1(float val){value.x = val;}

					void animateComponent2(float val){value.y = val;}

					void animateComponent3(float val){value.z = val;}
			};

			struct Vector4Uniform : public Uniform{
					Vector4 value;

					Vector4Uniform(std::string name, Vector4 value){
							this->name = name;
							this->value = value;
							this->type = Uniform::VECTOR_4;
					}

					void animateComponent1(float val){value.x = val;}

					void animateComponent2(float val){value.y = val;}

					void animateComponent3(float val){value.z = val;}

					void animateComponent4(float val){value.w = val;}
			};

			struct TextureUniform : public Uniform{
					Texture *value = nullptr;
					bool animatable = false;

					TextureUniform(std::string name, Texture *value, bool animatable){
							this->name = name;
							this->value = value;
							this->animatable = animatable;
							this->type = Uniform::TEXTURE;
					}
			};

			Material(std::string, bool = false);
			~Material();
			void update();
			Uniform* getUniform(std::string);
			inline Uniform* getUniform(int i){return uniforms[i];}
			inline void addVariable(std::string name, int value){uniforms.push_back(new IntUniform(name, value));}
			inline void addVariable(std::string name, bool value){uniforms.push_back(new BoolUniform(name, value));}
			inline void addVariable(std::string name, float value){uniforms.push_back(new BoolUniform(name, value));}
			inline void addVariable(std::string name, Vector2 value){uniforms.push_back(new Vector2Uniform(name, value));}
			inline void addVariable(std::string name, Vector3 value){uniforms.push_back(new Vector3Uniform(name, value));}
			inline void addVariable(std::string name, Vector4 value){uniforms.push_back(new Vector4Uniform(name, value));}
			inline void addVariable(std::string name, Texture *value, bool animatable){uniforms.push_back(new TextureUniform(name, value, animatable));}
			inline Shader* getShader(){return shader;}
		private:
			std::vector<Uniform*> uniforms;
			Shader *shader = nullptr;
	};
}

#endif
