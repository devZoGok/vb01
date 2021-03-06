#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include<glad.h>
#include<glfw3.h>
#include"texture.h"
#include<iostream>

using namespace std;

namespace vb01{
	Texture::Texture(int width, int height, bool shadowMap){
		this->width=width;
		this->height=height;
		texture=new u32;

		glGenTextures(1,&texture[0]);
		glBindTexture(GL_TEXTURE_2D,texture[0]);
		if(shadowMap){
			glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
			float borderColor[]={1,1,1,1};
			glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
		}
		else{
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,width,height,0,GL_RGB,GL_FLOAT,NULL);
			//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		}
	}

	Texture::Texture(string path[],int numPaths,int updateRate,bool flip){
		this->numFrames=numPaths;
		this->updateRate=updateRate;

		texture=new u32[numPaths];

		for(int i=0;i<numPaths;i++){
			bool png=false;
			int length=path[i].length();

			if(path[i].substr(length-4,string::npos)==".png")
				png=true;

			glGenTextures(1,&texture[i]);
			glBindTexture(GL_TEXTURE_2D,texture[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			stbi_set_flip_vertically_on_load(flip);

			data=stbi_load(path[i].c_str(),&width,&height,&numChannels,0);

			glTexImage2D(GL_TEXTURE_2D,0,png?GL_RGBA:GL_RGB,width,height,0,png?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,data);
			//glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
	}

	Texture::Texture(string paths[6],bool flip){
		this->type=TextureType::TEXTURE_CUBEMAP;
		texture=new u32;

		glGenTextures(1,&texture[0]);
		glBindTexture(GL_TEXTURE_CUBE_MAP,texture[0]);

		stbi_set_flip_vertically_on_load(flip);

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

	Texture::Texture(int width,bool depth){
		this->width=width;
		this->type=TextureType::TEXTURE_CUBEMAP;
		texture=new u32;

		glGenTextures(1,&texture[0]);
		glBindTexture(GL_TEXTURE_CUBE_MAP,texture[0]);

		for(int i=0;i<6;i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,depth?GL_DEPTH_COMPONENT:GL_RGB,width,width,0,depth?GL_DEPTH_COMPONENT:GL_RGB,GL_FLOAT,NULL);	
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);

	}

	Texture::Texture(FT_Face &face, char ch){
		texture=new u32;

		glGenTextures(1,&texture[0]);
		glBindTexture(GL_TEXTURE_2D,texture[0]);
		glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	Texture::~Texture(){
		glBindTexture(type==TEXTURE_2D?GL_TEXTURE_2D:GL_TEXTURE_CUBE_MAP,0);
		if(numFrames>0){
			for(int i=0;i<numFrames;i++)
				glDeleteTextures(1,&texture[i]);
			delete[] texture;
		}
		else{
			glDeleteTextures(1,&texture[0]);
			delete texture;
		}
	}

	void Texture::select(int id){
		glActiveTexture(GL_TEXTURE0+id);
		glBindTexture(type==TEXTURE_2D?GL_TEXTURE_2D:GL_TEXTURE_CUBE_MAP,texture[numFrames>0?frame:0]);
	}

	void Texture::update(int id){
		if(numFrames>0){
			if(getTime()-lastUpdateTime>updateRate){
				frame++;
				if(frame==numFrames)
					frame=0;
				lastUpdateTime=getTime();
			}
		}
		select(id);
	}
}
