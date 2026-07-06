#include "fontAsset.h"

namespace vb01{
	const FontAsset::Glyph& FontAsset::getGlyph(int id){
		for(Glyph &glyph : glyphs)
			if(glyph.id == id)
				return (const Glyph&) glyph;
	}
}
