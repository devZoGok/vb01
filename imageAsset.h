#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

namespace vb01{
		struct ImageAsset : public Asset{
				ImageAsset(std::string path, u8 *image, int width, int height){
						this->path = path;
						this->image = image;
						this->width = width;
						this->height = height;
				}

				u8 *image;
				int width, height;
		};
}

#endif
