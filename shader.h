#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glm/mat4x4.hpp>

namespace vb01{
	class Shader{
		public:
			Shader(std::string,std::string);
			void setNumLights(int);
			void use();
			void setMat4(glm::mat4,std::string);
			void setVec3(glm::vec3,std::string);
			void setFloat(float,std::string);
			void setBool(bool,std::string);
			void setInt(int,std::string);
		private:
			enum ErrorType{VERTEX,FRAGMENT,PROGRAM};
			void checkCompileErrors(unsigned int,ErrorType);
			unsigned int id;
			std::string vString,fString;
	};
}

#endif
