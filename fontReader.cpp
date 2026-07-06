#include "fontReader.h"
#include "fontAsset.h"
#include "root.h"

#include "stb_image.h"

#include <iostream>

#include <glad.h>

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

		vector<FontAsset::Glyph> glyphs;
		Root *root = Root::getSingleton();
		u32 *buffer = root->getGuiTextureBuffer();

		for(u16 i = firstChar; i < lastChar; i++){
			if(FT_Load_Char(face, i, FT_LOAD_RENDER)){
				cout << "Could not load glyph\n";
				continue;
			}

			FontAsset::Glyph glyph = FontAsset::Glyph(
					i, 
					face->glyph->bitmap.buffer, 
					Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
					Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top), 
					face->glyph->advance.x, 
					-1
			);
			root->initTextureDataOnGpu(buffer, glyph.data, glyph.size.x, glyph.size.y, glyph.layerId);

			glyphs.push_back(glyph);
		}

		FontAsset *font = new FontAsset;
		font->path = path;
		font->face = face;
		font->glyphs = glyphs;

		return font;
	}
}
