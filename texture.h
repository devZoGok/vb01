#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <ft2build.h>
#include "util.h"
#include FT_FREETYPE_H

namespace vb01{
	class Texture{
		public:
			enum TextureType{TEXTURE_2D, TEXTURE_CUBEMAP};
			enum TextureTypeId{DIFFUSE, NORMAL, SPECULAR, PARALLAX, ENVIRONMENT};

			~Texture();
			Texture(int, int, bool = true);
			Texture(std::string[], int, int = 0, bool = false);
			Texture(std::string[6], bool = false);
			Texture(int, bool = true);
			Texture(FT_Face&, char);
			void select(int = 0);
			void update(int = 0);
			inline unsigned int* getTexture(int i = 0){return &(texture[i]);}
			inline std::string getPath(){return path;}
			inline int getNumFrames(){return numFrames;}
		private:
			TextureType type = TextureType::TEXTURE_2D;
			u32 *texture = nullptr;
			s32 updateRate = 0, numFrames = 0,frame = 0;
			s64 lastUpdateTime = 0;
			int width, height, numChannels;
			float weight;
			u8 *data;
			std::string path = "", paths[6];

			void createCubemap(bool, bool);
			void updateFrame();
			inline int getNextFrame(int frameId){return (frameId + 1 < numFrames ? frameId + 1 : 0);}
	};
}

#endif
