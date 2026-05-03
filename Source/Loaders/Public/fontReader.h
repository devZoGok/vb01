#ifndef FONT_READER_H
#define FONT_READER_H

#include "abstractAssetReader.h"
#include "util.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vb01{
		class FontReader : public AbstractAssetReader{
				public:
						static FontReader* getSingleton();
						Asset* readAsset(std::string);
						inline void setPixelWidth(int w){this->pixelWidth = w;}
						inline void setPixelHeight(int h){this->pixelHeight = h;}
						inline int getPixelWidth(){return pixelWidth;}
						inline int getPixelHeight(){return pixelHeight;}
						inline FT_Library& getFT(){return ft;}
				private:
						FontReader();

						int pixelWidth = 0, pixelHeight = 100;
						FT_Library ft;
						FT_Face face;
		};
}

#endif
