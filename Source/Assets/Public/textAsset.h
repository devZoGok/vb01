#ifndef TEXT_ASSET_H
#define TEXT_ASSET_H

#include "asset.h"

namespace vb01{
		struct TextAsset : public Asset{
				std::string text;
		};
}

#endif
