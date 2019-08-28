#include<glad.h>
#include<glfw3.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include"shader.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

namespace vb01{
	Shader::Shader(string vertShaderPath, string fragShaderPath){
		ifstream vertShaderFile,fragShaderFile;
		vertShaderFile.open(vertShaderPath);
		fragShaderFile.open(fragShaderPath);
		
		stringstream vertShaderStream,fragShaderStream;
		vertShaderStream<<vertShaderFile.rdbuf();
		fragShaderStream<<fragShaderFile.rdbuf();
		
		vertShaderFile.close();
		fragShaderFile.close();
		
		string v=vertShaderStream.str();
		string f=fragShaderStream.str();

		const char *vertShaderSource=v.c_str();
		const char *fragShaderSource=f.c_str();

		unsigned int vert,frag;
		vert=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert,1,&vertShaderSource,NULL);
		glCompileShader(vert);
		checkCompileErrors(vert,VERTEX);

		frag=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag,1,&fragShaderSource,NULL);
		glCompileShader(frag);
		checkCompileErrors(frag,FRAGMENT);

		id=glCreateProgram();
		glAttachShader(id,vert);
		glAttachShader(id,frag);
		glLinkProgram(id);
		checkCompileErrors(id,PROGRAM);

		glDeleteShader(vert);
		glDeleteShader(frag);
	}
	
	void Shader::checkCompileErrors(unsigned int shader, ErrorType type){
		int success;
		char infoLog[1024];
		switch(type){
			case PROGRAM:
				glGetProgramiv(shader,GL_LINK_STATUS,&success);
				if(!success){
					glGetProgramInfoLog(shader,1024,NULL,infoLog);
					cout<<"ERROR::PROGRAM_LINK_ERROR: "<<type<<endl<<infoLog<<endl;
				}
				break;
			default:
				glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
				if(!success){
					const char *sh=(type==VERTEX?"VERTEX":"FRAGMENT");
					glGetShaderInfoLog(shader,1024,NULL,infoLog);
					cout<<"ERROS::SHADER_COMPILE_ERROR::"<<sh<<endl<<infoLog<<endl;
				}
				break;
		}
	}

	void Shader::use(){glUseProgram(id);}

	void Shader::setMat4(mat4 mat, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());
		glUniformMatrix4fv(uniformLoc,1,GL_FALSE,value_ptr(mat));
	}

	void Shader::setBool(bool b, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());	
		glUniform1i(uniformLoc,(int)b);
	}
}
