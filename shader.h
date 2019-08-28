#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glm/mat4x4.hpp>

namespace vb01{
	class Shader{
		public:
			Shader(std::string,std::string);
			void use();
			void setMat4(glm::mat4,std::string);
			void setBool(bool,std::string);
		private:
			enum ErrorType{VERTEX,FRAGMENT,PROGRAM};
			void checkCompileErrors(unsigned int,ErrorType);
			unsigned int id;
	};
}

#endif
