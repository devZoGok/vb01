#include "root.h"
#include "text.h"
#include "texture.h"
#include "node.h"
#include "material.h"
#include "assetManager.h"

using namespace std;

namespace vb01{
	Text::Text(string fontPath, wstring e, u16 firstChar, u16 lastChar){
		font = (FontAsset*)AssetManager::getSingleton()->getAsset(fontPath);
		setEntry(e);
	}

	wstring Text::getEntry(){
		wstring entry;

		for(Character &charac : characters)
			entry += charac.ch;

		return entry;
	}

	void Text::setEntry(wstring entry){
		for(Character &ch : characters)
			delete ch.material;

		characters.clear();

		for(wchar_t ch : entry){
			Material *mat = new Material(Root::getSingleton()->getGuiShader());
			mat->addTexUniform("glyphTexture", new Texture(font->path, ch), false, false);
			characters.push_back(Character(ch, mat));
		}
	}

	void Text::update(){
		Vector2 advanceOffset = Vector2::VEC_ZERO;
		Vector3 scale = node->getScale();
		int numShifts = 6, startId = (leftToRight ? 0 : getLength() - 1);

		for(int i = startId; (leftToRight ? (i < getLength()) : (i >= 0)); (leftToRight ? i++ : i--)){
			Character &charac = characters[i];
			const FontAsset::Glyph &currGlyph = font->getGlyph(charac.ch);
			charac.offset = Vector2( (currGlyph.bearing.x + advanceOffset.x),  (advanceOffset.y - currGlyph.bearing.y));

			if(horizontal)
				advanceOffset.x += (currGlyph.advance >> numShifts);
			else
				advanceOffset.y += currGlyph.size.y + (currGlyph.size.y - currGlyph.bearing.y);
		}
	}
}
