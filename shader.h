#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/mat4x4.hpp>

#include "shaderAsset.h"
#include "vector.h"
#include "util.h"

namespace vb01{
	class Shader{
		public:
			enum ErrorType{VERTEX_ERROR, GEOMETRY_ERROR, FRAGMENT_ERROR, PROGRAM_ERROR};
			enum ShaderType{VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};

			Shader(){}
			Shader(std::string, bool = false);
			Shader(std::string, std::string);
			Shader(std::string, std::string, std::string);
			~Shader(){}
			std::string getName();
			void use();
			void loadShaders();
			void setMat4(glm::mat4, std::string);
			void setVec4(Vector4, std::string);
			void setVec3(Vector3, std::string);
			void setVec2(Vector2, std::string);
			void setFloat(float, std::string);
			void setBool(bool, std::string);
			void setInt(int, std::string);
			void setUnsignedInt(u32, std::string);
			void editShader(ShaderType, int, std::string){}
			inline bool isGeometry(){return geometry;}
		private:
			void initShaders(std::string, std::string, std::string);
			void replaceLine(ShaderType, int, std::string);
			void pushShader(u32&, std::string&, int, ErrorType);
			void checkCompileErrors(u32, ErrorType);
			u32 id;
			bool geometry = false;
			ShaderAsset *vString = nullptr, *fString = nullptr, *gString = nullptr;

			friend class ShaderTest;
	};
}

#endif
