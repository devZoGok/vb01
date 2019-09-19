#ifndef TEXTURE_H
#define TEXTURE_H

#include<string>
#include<ft2build.h>
#include FT_FREETYPE_H

namespace vb01{
	class Texture{
		public:
			enum TextureType{TEXTURE_2D,TEXTURE_CUBEMAP};

			Texture();
			Texture(FT_Face&,char);
			Texture(std::string);
			Texture(std::string[6]);
			void select();
			inline unsigned int* getTexture(){return &texture;}
		private:
			TextureType type;
			unsigned int texture;
			int width,height,numChannels;
			float weight;
			unsigned char *data;
	};
}

#endif
