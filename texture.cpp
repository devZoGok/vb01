#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include<glad.h>
#include<glfw3.h>
#include"texture.h"

using namespace std;

namespace vb01{
	Texture::Texture(string path){
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

	void Texture::select(){
		glBindTexture(GL_TEXTURE_2D,texture);
	}
}
