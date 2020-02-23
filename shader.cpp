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
	Shader::Shader(string vertShaderPath, string fragShaderPath, string geoShaderPath){
		geometry=(geoShaderPath!="");

		ifstream vertShaderFile,geoShaderFile,fragShaderFile;
		vertShaderFile.open(vertShaderPath);
		geoShaderFile.open(geoShaderPath);
		fragShaderFile.open(fragShaderPath);
		
		stringstream vertShaderStream,geoShaderStream,fragShaderStream;
		vertShaderStream<<vertShaderFile.rdbuf();
		geoShaderStream<<geoShaderFile.rdbuf();
		fragShaderStream<<fragShaderFile.rdbuf();
		
		vertShaderFile.close();
		geoShaderFile.close();
		fragShaderFile.close();
		
		vString=vertShaderStream.str();
		gString=geoShaderStream.str();
		fString=fragShaderStream.str();

		loadShaders();
	}

	Shader::~Shader(){}

	void Shader::editShader(bool vertexShader, char firstChar, char secondChar, string insertion){
		int firstCharId=-1,secondCharId=-1;
		for(int i=0;i<fString.length()&&(firstCharId==-1||secondCharId==-1);i++){
			if(fString.c_str()[i]==firstChar)
				firstCharId=i;
			if(fString.c_str()[i]==secondChar)
				secondCharId=i;
		}
		string *shaderString=(vertexShader?&vString:&fString);
		(*shaderString)=(*shaderString).substr(0,firstCharId+1)+insertion+(*shaderString).substr(secondCharId,string::npos);

		loadShaders();

	}

	void Shader::loadShaders(){
		const char *vertShaderSource=vString.c_str();
		const char *geoShaderSource=gString.c_str();
		const char *fragShaderSource=fString.c_str();

		unsigned int vert,geo,frag;
		vert=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert,1,&vertShaderSource,NULL);
		glCompileShader(vert);
		checkCompileErrors(vert,VERTEX);

		if(geometry){
			geo=glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geo,1,&geoShaderSource,NULL);
			glCompileShader(geo);
			checkCompileErrors(geo,GEOMETRY);
		}

		frag=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag,1,&fragShaderSource,NULL);
		glCompileShader(frag);
		checkCompileErrors(frag,FRAGMENT);

		id=glCreateProgram();
		glAttachShader(id,vert);
		if(geometry) glAttachShader(id,geo);
		glAttachShader(id,frag);
		glLinkProgram(id);
		checkCompileErrors(id,PROGRAM);

		glDeleteShader(vert);
		if(geometry) glDeleteShader(geo);
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
					const char *sh;
					switch(type){
						case VERTEX:
							sh="VERTEX";
							break;
						case GEOMETRY:
							sh="GEOMETRY";
							break;
						case FRAGMENT:
							sh="FRAGMENT";
							break;
					}
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

	void Shader::setVec4(Vector4 vec, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());
		glUniform4f(uniformLoc,vec.x,vec.y,vec.z,vec.w);
	}

	void Shader::setVec3(Vector3 vec, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());
		glUniform3f(uniformLoc,vec.x,vec.y,vec.z);
	}

	void Shader::setVec2(Vector2 vec, string var){
		int uniformLoc=glGetUniformLocation(id,var.c_str());
		glUniform2f(uniformLoc,vec.x,vec.y);
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
