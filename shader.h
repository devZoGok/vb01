#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glm/mat4x4.hpp>
#include"vector.h"
#include"util.h"

namespace vb01{
	class Shader{
		public:
			enum ErrorType{VERTEX_ERROR, GEOMETRY_ERROR, FRAGMENT_ERROR, PROGRAM_ERROR};
			enum ShaderType{VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};

			Shader(std::string, std::string, std::string = "");
			~Shader();
			void setNumLights(int);
			void use();
			void setMat4(glm::mat4, std::string);
			void setVec4(Vector4, std::string);
			void setVec3(Vector3, std::string);
			void setVec2(Vector2, std::string);
			void setFloat(float, std::string);
			void setBool(bool, std::string);
			void setInt(int, std::string);
			void editShader(ShaderType, int, std::string);
		private:
			void initShaders(std::string, std::string, std::string);
			void replaceLine(ShaderType, int, std::string);
			void loadShaders();
			void pushShader(u32&, std::string&, int, ErrorType);
			void checkCompileErrors(u32, ErrorType);
			unsigned int id;
			bool geometry = false;
			std::string vString, fString, gString;
	};
}

#endif
