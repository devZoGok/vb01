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
		
		vString=vertShaderStream.str();
		fString=fragShaderStream.str();
		setNumLights(1);
	}

	void Shader::setNumLights(int numLights){
		int firstCharId=-1,secondCharId=-1;
		for(int i=0;i<fString.length()&&(firstCharId==-1||secondCharId==-1);i++){
			if(fString.c_str()[i]=='=')
				firstCharId=i;
			if(fString.c_str()[i]==';')
				secondCharId=i;
		}
		fString=fString.substr(0,firstCharId+1)+to_string(numLights)+fString.substr(secondCharId,string::npos);

		const char *vertShaderSource=vString.c_str();
		const char *fragShaderSource=fString.c_str();

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

	void Shader::setFloat(float fl, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());
		glUniform1f(uniformLoc,fl);
	}

	void Shader::setVec3(vec3 vec, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());
		glUniform3f(uniformLoc,vec.x,vec.y,vec.z);
	}

	void Shader::setBool(bool b, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());	
		glUniform1i(uniformLoc,(int)b);
	}

	void Shader::setInt(int i, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());	
		glUniform1i(uniformLoc,i);
	}
}
