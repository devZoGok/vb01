#ifndef FONT_ASSET_H
#define FONT_ASSET_H

#include "asset.h"
#include "util.h"

#include <vector>

#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vb01{
	class Texture;

	struct FontAsset : public Asset{
		struct Glyph{
			u8 *data;
			u32 advance;
			int id, bufferId, layerId;
			Vector2 size, bearing;

			Glyph(int i, u8 *d, Vector2 s, Vector2 b, u32 adv, int bid, int lid) : id(i), data(d), size(s), bearing(b), advance(adv), bufferId(bid), layerId(lid){}
		};

		const Glyph& getGlyph(int);

		FT_Face face;
		std::vector<Glyph> glyphs;
	};
}

#endif
