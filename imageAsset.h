#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

namespace vb01{
		struct ImageAsset : public Asset{
				ImageAsset(std::string path, u8 *image){
						this->path = path;
						this->image = image;
				}

				u8 *image;
		};
}

#endif
