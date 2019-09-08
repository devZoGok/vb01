#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include<glad.h>
#include<glfw3.h>
#include"texture.h"
#include<iostream>

using namespace std;

namespace vb01{
	Texture::Texture(string path){
		this->type=TextureType::TEXTURE_2D;

		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		data=stbi_load(path.c_str(),&width,&height,&numChannels,0);

		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	Texture::Texture(string paths[6]){
		this->type=TextureType::TEXTURE_CUBEMAP;

		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP,texture);
		for(int i=0;i<6;i++){
			data=stbi_load(paths[i].c_str(),&width,&height,&numChannels,0);
			if(data){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);	
				stbi_image_free(data);
			}
			else{
				cout<<"Failed to read cubemap texture "<<i<<endl;
				exit(-1);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

	}

	void Texture::select(){
		glBindTexture(type==TEXTURE_2D?GL_TEXTURE_2D:GL_TEXTURE_CUBE_MAP,texture);
	}
}
