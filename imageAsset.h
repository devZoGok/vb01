#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

namespace vb01{
	struct ImageAsset : public Asset{
		ImageAsset(std::string p, u8 *img, int w, int h, int nc, int lid) : 
			image(img), 
			width(w), 
			height(h), 
			numChannels(nc), 
			layerId(lid)
		{
			this->path = p;
		}

		u8 *image;
		int width, height, numChannels, layerId;
	};
}

#endif
