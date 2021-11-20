#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.h"

#include "glad.h"
#include <glfw3.h>
#include <iostream>

using namespace std;

namespace vb01{
	Texture::Texture(int width, int height, bool shadowMap){
		this->width = width;
		this->height = height;
		texture = new u32;

		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		if(shadowMap){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = {1, 1, 1, 1};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}
		else{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	Texture::Texture(string paths[], int numPaths, bool cubemap, int mipmapLevel, bool flip){
			this->numFrames = numPaths;
			this->cubemap = cubemap;
			this->paths = new string[numPaths];

			for(int i = 0; i < numPaths; i++)
				this->paths[i] = paths[i];

			if(cubemap){
				texture = new u32;
				createCubemap(false, flip);
			}
			else{
				texture = new u32[numPaths];
				create2DTexture(flip);
			}
	}

	void Texture::create2DTexture(bool flip){
		mixRatio = .1;

		for(int i = 0; i < numFrames; i++){
			bool png = false;
			int length = paths[i].length();

			if(paths[i].substr(length - 4, string::npos) == ".png")
				png = true;

			glGenTextures(1, &texture[i]);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_set_flip_vertically_on_load(flip);

			data = stbi_load(paths[i].c_str(), &width, &height, &numChannels, 0);

			glTexImage2D(GL_TEXTURE_2D, 0, png ? GL_RGBA : GL_RGB, width, height, 0, png ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
	}

	Texture::Texture(int width, bool depth, int mipmapLevel){
		this->width = width;
		this->mipmapLevel = mipmapLevel;
		texture = new u32;

		createCubemap(true, false);
	}

	void Texture::createCubemap(bool depth, bool flip){
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture[0]);

		if(flip)
			stbi_set_flip_vertically_on_load(flip);

		for(int i = 0; i < 6; i++){
			if(!depth){
				data = stbi_load(paths[i].c_str(), &width, &height, &numChannels, 0);

				if(data){
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	
					stbi_image_free(data);
				}
				else{
					cout << "Failed to read cubemap texture " << i << endl;
					exit(-1);
				}

				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			}
			else{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);	
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture::Texture(FT_Face &face, char ch){
		texture = new u32;

		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture(){
		glBindTexture(cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 0);

		if(numFrames > 0){
			for(int i = 0; i < numFrames; i++)
				glDeleteTextures(1, &texture[i]);

			delete[] texture;
		}
		else{
			glDeleteTextures(1, &texture[0]);
			delete texture;
		}
	}

	void Texture::animate(float value, KeyframeChannel keyframeChannel){
		switch(keyframeChannel.type){
			case KeyframeChannel::TEXTURE_MIX_RATIO:
				mixRatio = value;
				break;
			case KeyframeChannel::TEXTURE_FRAME_A:
				frameA = (int)value;
				break;
			case KeyframeChannel::TEXTURE_FRAME_B:
				frameB = (int)value;
				break;
		}
	}

	void Texture::update(int id){
		select(id, frameA);

		if(numFrames > 0)
			select(id + 1, frameB);
	}

	void Texture::select(int id, int fr){
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texture[fr]);
	}
}
