#include "glad.h"
#include <glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "assetManager.h"

using namespace std;
using namespace glm;

namespace vb01{
	Shader::Shader(string shaderPath, bool geometry){
		this->geometry = geometry;

		initShaders(shaderPath + ".vert", shaderPath + ".frag", shaderPath + ".geo");
		loadShaders();
	}

	Shader::Shader(string vertShader, string fragShader){
		this->geometry = false;

		initShaders(vertShader, fragShader, "");
		loadShaders();
	}

	Shader::Shader(string vertShader, string fragShader, string geoShader){
		this->geometry = true;

		initShaders(vertShader, fragShader, geoShader);
		loadShaders();
	}

	string Shader::getName(){
		int dirId = vString->path.find_last_of('/');
		string name = (dirId != -1 ? vString->path.substr(dirId + 1) : vString->path);

		int dotId = name.find_last_of('.');

		if(dotId != -1)
			name = name.substr(0, dotId);

		return name;
	}

	void Shader::initShaders(string vertShaderPath, string fragShaderPath, string geoShaderPath){
		AssetManager *am = AssetManager::getSingleton();
		vString = (ShaderAsset*)am->getAsset(vertShaderPath);
		fString = (ShaderAsset*)am->getAsset(fragShaderPath);

		if(geometry)
			gString = (ShaderAsset*)am->getAsset(geoShaderPath);
	}

	void Shader::pushShader(u32 &type, string &sString, int glType, ErrorType errorType){
		type = glCreateShader(glType);
		const char *shaderSource = sString.c_str();
		glShaderSource(type, 1, &shaderSource, NULL);
		glCompileShader(type);
		checkCompileErrors(type, errorType);
	}

	void Shader::loadShaders(){
		u32 vert, geo, frag;
		pushShader(vert, vString->shaderString, GL_VERTEX_SHADER, VERTEX_ERROR);
		pushShader(frag, fString->shaderString, GL_FRAGMENT_SHADER, FRAGMENT_ERROR);

		if(geometry)
			pushShader(geo, gString->shaderString, GL_GEOMETRY_SHADER, GEOMETRY_ERROR);

		id = glCreateProgram();
		glAttachShader(id, vert);
		glAttachShader(id, frag);

		if(geometry)
			glAttachShader(id, geo);

		glLinkProgram(id);
		checkCompileErrors(id, PROGRAM_ERROR);

		glDeleteShader(vert);
		glDeleteShader(frag);

		if(geometry) 
			glDeleteShader(geo);
	}
	
	void Shader::checkCompileErrors(u32 shader, ErrorType type){
		int success;
		const int infoLogLen = 1024;
		char infoLog[infoLogLen];

		switch(type){
			case PROGRAM_ERROR:
				glGetProgramiv(shader, GL_LINK_STATUS, &success);

				if(!success){
					glGetProgramInfoLog(shader, infoLogLen, NULL, infoLog);
					cout << "ERROR::PROGRAM_LINK_ERROR: " << type << endl << infoLog << endl;
				}

				break;
			default:
				glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

				if(!success){
					const char *sh;

					switch(type){
						case VERTEX_ERROR:
							sh = "VERTEX";
							break;
						case GEOMETRY_ERROR:
							sh = "GEOMETRY";
							break;
						case FRAGMENT_ERROR:
							sh = "FRAGMENT";
							break;
					}

					glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
					cout << "ERROS::SHADER_COMPILE_ERROR::" << sh << endl << infoLog << endl;
				}

				break;
		}
	}

	void Shader::use(){glUseProgram(id);}

	void Shader::setMat4(mat4 mat, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value_ptr(mat));
	}

	void Shader::setFloat(float fl, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());
		glUniform1f(uniformLoc, fl);
	}

	void Shader::setVec4(Vector4 vec, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());
		glUniform4f(uniformLoc, vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::setVec3(Vector3 vec, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());
		glUniform3f(uniformLoc, vec.x, vec.y, vec.z);
	}

	void Shader::setVec2(Vector2 vec, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());
		glUniform2f(uniformLoc, vec.x, vec.y);
	}

	void Shader::setBool(bool b, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());	
		glUniform1i(uniformLoc, (int)b);
	}

	void Shader::setInt(int i, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());	
		glUniform1i(uniformLoc, i);
	}

	void Shader::setUnsignedInt(u32 i, string var){
		int uniformLoc = glGetUniformLocation(id, var.c_str());	
		glUniform1ui(uniformLoc, i);
	}
}
