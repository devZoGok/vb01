#ifndef TEXTURE_H
#define TEXTURE_H

#include<string>
#include<ft2build.h>
#include"util.h"
#include FT_FREETYPE_H

namespace vb01{
	class Texture{
		public:
			enum TextureType{TEXTURE_2D,TEXTURE_CUBEMAP};

			~Texture();
			Texture(int,int,bool=true);
			Texture(std::string,bool=false);
			Texture(std::string[6],bool=false);
			Texture(int);
			Texture(FT_Face&,char);
			void select(int=0);
			inline unsigned int* getTexture(){return &texture;}
			inline std::string getPath(){return path;}
		private:
			TextureType type=TextureType::TEXTURE_2D;
			u32 texture;
			s64 lastUpdateTime=0;
			int width,height,numChannels;
			float weight;
			u8 *data;
			std::string path="",paths[6];
	};
}

#endif
