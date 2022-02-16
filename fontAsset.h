#ifndef FONT_ASSET_H
#define FONT_ASSET_H

#include "asset.h"

namespace vb01{
		struct FontAsset : public Asset{
				FT_Face face;
		};
}

#endif
