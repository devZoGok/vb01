#ifndef TEXTURE_H
#define TEXTURE_H

#include<string>

namespace vb01{
	class Texture{
		public:
			enum TextureType{TEXTURE_2D,TEXTURE_CUBEMAP};

			Texture(std::string);
			Texture(std::string[6]);
			void select();
		private:
			TextureType type;
			unsigned int texture;
			int width,height,numChannels;
			float weight;
			unsigned char *data;
	};
}

#endif
