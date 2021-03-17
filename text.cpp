#include "root.h"
#include "text.h"
#include "shader.h"
#include "texture.h"
#include "node.h"
#include "material.h"

#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

namespace vb01{
	Text::Text(string fontPath, wstring entry){
		initFont(fontPath);
		setText(entry);
	}

	Text::~Text(){
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void Text::initFont(string fontPath){
		FT_Library ft;
		FT_Face face;
		if(FT_Init_FreeType(&ft))
			cout << "Couldn't init Freetype\n";	
		if(FT_New_Face(ft, fontPath.c_str(), 0, &face))
			cout << "Could not load font\n";

		FT_Set_Pixel_Sizes(face, 0, 100);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for(u16 i = 0; i < u16(0 - 1); i++){
			if(FT_Load_Char(face, i, FT_LOAD_RENDER)){
				cout << "Could not load glyph\n";
				continue;
			}
			Glyph c;
			c.ch = i;
			c.texture = new Texture(face, i);		
			c.size = Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			c.bearing = Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			c.advance = face->glyph->advance.x;
			characters.push_back(c);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void Text::update(){
		Root *root = Root::getSingleton();
		int width = root->getWidth(), height = root->getHeight();

		material->update();
		Shader *shader = material->getShader();
		shader->setVec2(Vector2(width, height), "screen");
		shader->setVec3(node->getPosition(), "pos");

		Vector2 advanceOffset = Vector2::VEC_ZERO;
		for(int i = 0; i < entry.length(); i++){
			Glyph *glyphPtr = getGlyph(entry[i]);
			if(!glyphPtr)
				continue;

			Glyph glyph = *glyphPtr;
			prepareGlyphs(glyph, advanceOffset);
			Vector2 size = glyph.size, bearing = glyph.bearing;
			if(horizontal)
				advanceOffset.x += (size.x + bearing.x);
			else
				advanceOffset.y += size.y;
		}
	}

	void Text::prepareGlyphs(Glyph glyph, Vector2 advanceOffset){
		Vector3 nodePos = node->getPosition();
		Vector2 origin = Vector2(nodePos.x, nodePos.y);
		origin = origin + (advanceOffset * scale);

		Vector2 size = glyph.size * scale, bearing = glyph.bearing * scale;
		float data[] = {
			origin.x + bearing.x, origin.y - bearing.y, 0, 0,
			origin.x + bearing.x + size.x, origin.y - bearing.y, 1, 0,
			origin.x + bearing.x + size.x, origin.y - bearing.y + size.y, 1, 1,

			origin.x + bearing.x + size.x, origin.y - bearing.y + size.y, 1, 1,
			origin.x + bearing.x, origin.y - bearing.y + size.y, 0, 1,
			origin.x + bearing.x, origin.y - bearing.y, 0, 0
		};

		renderGlyphs(glyph, data, sizeof(data));
	}

	void Text::renderGlyphs(Glyph glyph, float data[], u32 dataSize){
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
		glyph.texture->select(2);
		glDrawArrays(GL_TRIANGLES, 0, 6);	
	}

	void Text::setText(wstring text){
		this->entry = text;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Text::Glyph* Text::getGlyph(u16 ch){
		Glyph *glyph = nullptr;
		for(Glyph &g : characters)
			if(g.ch == ch)
				glyph = &g;
		return glyph;
	}
}
