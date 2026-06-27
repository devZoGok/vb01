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

	class Material{
		public:
			struct Uniform : public Animatable{
				enum Type{
					INT,
					BOOL,
					FLOAT,
					VECTOR_2,
					VECTOR_3,
					VECTOR_4,
					TEXTURE
				};

				Uniform(std::string n, Type t, bool un) : Animatable(Animatable::MATERIAL_UNIFORM, n), name(n), type(t), uniform(un){}
 
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

				Type type;
				bool uniform;
				std::string name;

				protected:
					virtual void animateComponent1(float val){}
					virtual void animateComponent2(float val){}
					virtual void animateComponent3(float val){}
					virtual void animateComponent4(float val){}
			};

			struct IntUniform : public Uniform{
				int value;

				IntUniform(std::string name, int val, bool unif) : Uniform(name, Uniform::INT, unif), value(val){}

				void animateComponent1(float val){value = (int)val;}
			};

			struct BoolUniform : public Uniform{
				bool value;

				BoolUniform(std::string name, bool val, bool unif) : Uniform(name, Uniform::BOOL, unif), value(val){}

				void animateComponent1(float val){value = (bool)val;}
			};

			struct FloatUniform : public Uniform{
				float value;

				FloatUniform(std::string name, float val, bool unif) : Uniform(name, Uniform::FLOAT, unif), value(val){}

				void animateComponent1(float val){value = val;}
			};

			struct Vector2Uniform : public Uniform{
				Vector2 value;

				Vector2Uniform(std::string name, Vector2 val, bool unif) : Uniform(name, Uniform::VECTOR_2, unif), value(val){}

				void animateComponent1(float val){value.x = val;}

				void animateComponent2(float val){value.y = val;}
			};

			struct Vector3Uniform : public Uniform{
				Vector3 value;

				Vector3Uniform(std::string name, Vector3 val, bool unif) : Uniform(name, Uniform::VECTOR_3, unif), value(val){}

				void animateComponent1(float val){value.x = val;}

				void animateComponent2(float val){value.y = val;}

				void animateComponent3(float val){value.z = val;}
			};

			struct Vector4Uniform : public Uniform{
				Vector4 value;

				Vector4Uniform(std::string name, Vector4 val, bool unif) : Uniform(name, Uniform::VECTOR_4, unif), value(val){}

				void animateComponent1(float val){value.x = val;}

				void animateComponent2(float val){value.y = val;}

				void animateComponent3(float val){value.z = val;}

				void animateComponent4(float val){value.w = val;}
			};

			struct TextureUniform : public Uniform{
				Texture *value = nullptr;
				bool animatable = false;

				TextureUniform(std::string name, Texture *val, bool anim, bool unif) : Uniform(name, Uniform::TEXTURE, unif), value(val), animatable(anim){}
			};

			Material(Shader*);
			~Material(){}
			void update();
			void setIntUniform(std::string, int);
			void setBoolUniform(std::string, bool);
			void setFloatUniform(std::string, float);
			void setVec2Uniform(std::string, Vector2);
			void setVec3Uniform(std::string, Vector3);
			void setVec4Uniform(std::string, Vector4);
			void setTexUniform(std::string, Texture*, bool);
			Uniform* getUniform(std::string);
			std::vector<Uniform*> getUniformsByType(Uniform::Type);
			inline Uniform* getUniform(int i){return uniforms[i];}
			inline void addIntUniform(std::string name, int value, bool uniform = true){uniforms.push_back(new IntUniform(name, value, uniform));}
			inline void addBoolUniform(std::string name, bool value, bool uniform = true){uniforms.push_back(new BoolUniform(name, value, uniform));}
			inline void addFloatUniform(std::string name, float value, bool uniform = true){uniforms.push_back(new FloatUniform(name, value, uniform));}
			inline void addVec2Uniform(std::string name, Vector2 value, bool uniform = true){uniforms.push_back(new Vector2Uniform(name, value, uniform));}
			inline void addVec3Uniform(std::string name, Vector3 value, bool uniform = true){uniforms.push_back(new Vector3Uniform(name, value, uniform));}
			inline void addVec4Uniform(std::string name, Vector4 value, bool uniform = true){uniforms.push_back(new Vector4Uniform(name, value, uniform));}
			inline void addTexUniform(std::string name, Texture *value, bool animatable, bool uniform = true){uniforms.push_back(new TextureUniform(name, value, animatable, uniform));}
			inline Shader* getShader(){return shader;}
			inline bool isTransparent(){return transparent;}
			inline void setTransparent(bool t){this->transparent = t;}
		private:
			std::vector<Uniform*> uniforms;
			Shader *shader = nullptr;
			bool transparent = false;
	};
}

#endif
