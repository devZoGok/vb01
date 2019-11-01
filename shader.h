#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glm/mat4x4.hpp>
#include"vector.h"

namespace vb01{
	class Shader{
		public:
			enum ErrorType{VERTEX,FRAGMENT,PROGRAM};

			Shader(std::string,std::string);
			~Shader();
			void setNumLights(int);
			void use();
			void setMat4(glm::mat4,std::string);
			void setVec4(Vector4,std::string);
			void setVec3(Vector3,std::string);
			void setVec2(Vector2,std::string);
			void setFloat(float,std::string);
			void setBool(bool,std::string);
			void setInt(int,std::string);
			void editShader(bool,char,char,std::string);
		private:
			void loadShaders();
			void checkCompileErrors(unsigned int,ErrorType);
			unsigned int id;
			std::string vString,fString;
	};
}

#endif
