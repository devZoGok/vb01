#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <ft2build.h>
#include "util.h"
#include FT_FREETYPE_H

namespace vb01{
	class Texture : public Animatable{
		public:
			enum TextureType{TEXTURE_2D, TEXTURE_CUBEMAP};
			enum TextureTypeId{DIFFUSE, NORMAL, SPECULAR, PARALLAX, ENVIRONMENT};

			~Texture();
			Texture(int, int, bool = true);
			Texture(std::string[], int, TextureTypeId = DIFFUSE, bool = false);
			Texture(std::string[6], bool = false);
			Texture(int, bool = true);
			Texture(FT_Face&, char);
			void select(int = 0, int = 0);
			void update(int = 0);
			void animate(float, KeyframeChannel);
			inline unsigned int* getTexture(int i = 0){return &(texture[i]);}
			inline std::string getPath(){return path;}
			inline int getNumFrames(){return numFrames;}
			inline float getMixRatio(){return mixRatio;}
			inline TextureTypeId getTextureTypeId(){return typeId;}
		private:
			TextureType type = TextureType::TEXTURE_2D;
			TextureTypeId typeId = DIFFUSE;
			u32 *texture = nullptr;
			s64 lastUpdateTime = 0;
			int width, height, numChannels;
			float mixRatio;
			u8 *data;
			std::string path = "", paths[6];

			void createCubemap(bool, bool);
			void updateFrame();
			inline int getNextFrame(int frameId){return (frameId + 1 < numFrames ? frameId + 1 : 0);}
	};
}

#endif
