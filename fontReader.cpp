#include "fontReader.h"
#include "fontAsset.h"

#include <iostream>

#include "glad.h"
#include <glfw3.h>

using namespace std;

namespace vb01{
		FontReader *fontReader = nullptr;

		FontReader* FontReader::getSingleton(){
				if(!fontReader)
						fontReader = new FontReader();

				return fontReader;
		}

		FontReader::FontReader() : AbstractAssetReader(){
			if(FT_Init_FreeType(&ft))
				cout << "Couldn't init Freetype\n";	
		}

		Asset* FontReader::readAsset(string path){
			if(FT_New_Face(ft, path.c_str(), 0, &face))
				cout << "Could not load font\n";

			FT_Set_Pixel_Sizes(face, pixelWidth, pixelHeight);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			FontAsset *font = new FontAsset;
			font->path = path;
			font->face = face;

			return font;
		}
}
