#ifndef TEXTURE_H
#define TEXTURE_H

#include<string>

namespace vb01{
	class Texture{
		public:
			Texture(std::string);
			void select();
		private:
			unsigned int texture;
			int width,height,numChannels;
			float weight;
			unsigned char *data;
	};
}

#endif
