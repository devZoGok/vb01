#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "util.h"
#include "animatable.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vb01{
	class Texture : public Animatable{
		public:
			~Texture();
			Texture(int, int, bool = true);
			Texture(std::string[], int, bool, int = 0, bool = false);
			Texture(int, bool = true, int = 0);
			Texture(FT_Face&, char);
			void select(int = 0, int = 0);
			void update(int = 0);
			void animate(float, KeyframeChannel);
			inline u32* getTexture(int i = 0){return &(texture[i]);}
			inline std::string* getPath(){return paths;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			inline int getNumFrames(){return numFrames;}
			inline int getMipmapLevel(){return mipmapLevel;}
			inline float getMixRatio(){return mixRatio;}
			inline bool isCubemap(){return cubemap;}
		private:
			bool cubemap = false;
			u32 *texture = nullptr;
			s64 lastUpdateTime = 0;
			int width, height, updateRate = 0, numFrames = 0, frameA = 0, frameB = 0, mipmapLevel = 1;
			float mixRatio;
			std::string *paths = nullptr;

			void createCubemap(bool, bool, bool);
			void create2DTexture(bool);
			inline int getNextFrame(int frameId){return (frameId + 1 < numFrames ? frameId + 1 : 0);}
	};
}

#endif
