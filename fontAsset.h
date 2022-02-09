#ifndef FONT_ASSET_H
#define FONT_ASSET_H

#include "asset.h"

namespace vb01{
		struct FontAsset : public Asset{
				u32 **glyphImages;
		};
}

#endif
