#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.h"
#include "assetManager.h"
#include "imageAsset.h"
#include "root.h"

#include <glad.h>
#include <glfw3.h>
#include <iostream>

using namespace std;

namespace vb01{
	Texture::Texture(int w, int h, bool shadowMap, bool hiRes) : Animatable(Animatable::TEXTURE), width(w), height(h), texture(new u32){
		if(shadowMap){
			/*
		texture = new u32;
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = {1, 1, 1, 1};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
			*/
		}
		else
			Root::getSingleton()->initTextureDataOnGpu(width, height);
	}

	Texture::Texture(string paths[], int np, bool cm, int ml, bool flip, string name) : Animatable(Animatable::TEXTURE, name), numFrames(np), cubemap(cm), mipmapLevel(ml){
		this->paths = new string[np];

		for(int i = 0; i < np; i++){
			this->paths[i] = paths[i];

			if(!cm){
				ImageAsset *imgAsset = (ImageAsset*)AssetManager::getSingleton()->getAsset(paths[i]);
				frames.push_back(Frame(nullptr, imgAsset->layerId));
			}
		}

		if(cm) Root::getSingleton()->createCubemap(false, true, paths, ml);
	}

	Texture::Texture(int w, bool depth, int ml, string name) : Animatable(Animatable::TEXTURE, name), width(w), mipmapLevel(ml), cubemap(true){
		string p[6]{""};
		Root::getSingleton()->createCubemap(depth, false, p, ml);
	}

	Texture::Texture(FT_Face face) : Animatable(Animatable::TEXTURE){
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
		delete paths;
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
		/*
		select(id, frameA);

		if(numFrames > 0)
			select(id + 1, frameB);
			*/
	}

	void Texture::select(int id, int fr){
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texture[fr]);
	}
}
