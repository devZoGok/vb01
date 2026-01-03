#ifndef FONT_ASSET_H
#define FONT_ASSET_H

#include "asset.h"

#include <vector>
#include <utility>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace vb01{
	class Texture;

	struct FontAsset : public Asset{
			FT_Face face;
			std::vector<std::pair<int, Texture*>> glyphTextures;
	};
}

#endif
