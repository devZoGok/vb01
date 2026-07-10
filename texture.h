#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "util.h"
#include "animatable.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vb01{
	struct ImageAsset;

	class Texture : public Animatable{
		public:
			struct Frame{
				int bufferId = 0, layerId = 0;

				Frame(int bid, int lid) : bufferId(bid), layerId(lid){}
			};

			~Texture();
			Texture(int, int, bool = true, bool = false);
			Texture(std::string[], int, bool, int = 0, bool = false, std::string = "");
			Texture(int, bool = true, int = 0, std::string = "");
			Texture(std::string, int);
			void select(int = 0, int = 0);
			void update(int = 0);
			void animate(float, KeyframeChannel);
			void loadImageData(ImageAsset*, bool, int = 0);
			inline const Frame& getFrame(int i){return frames[i];}
			inline u32* getTexture(int i = 0){return &(texture[i]);}
			inline std::string* getPath(){return paths;}
			inline int getNumFrames(){return numFrames;}
			inline int getMipmapLevel(){return mipmapLevel;}
			inline float getMixRatio(){return mixRatio;}
			inline bool isCubemap(){return cubemap;}
			inline bool isTransparent(){return png;}
		private:
			bool cubemap = false, png = false;
			u32 *texture = nullptr;
			s64 lastUpdateTime = 0;
			int width, height, updateRate = 0, numFrames = 0, frameA = 0, frameB = 0, mipmapLevel = 1;
			float mixRatio;
			std::string *paths = nullptr;
			std::vector<Frame> frames;

			inline int getNextFrame(int frameId){return (frameId + 1 < numFrames ? frameId + 1 : 0);}
	};
}

#endif
