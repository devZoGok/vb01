#ifndef IMAGE_ASSET_H
#define IMAGE_ASSET_H

#include "asset.h"

namespace vb01{
	struct ImageAsset : public Asset{
		ImageAsset(std::string p, u8 *img, int w, int h, int nc, int bid, int lid, bool ltg) : 
			image(img), 
			width(w), 
			height(h), 
			numChannels(nc), 
			bufferId(lid),
			layerId(lid),
			loadedToGpu(ltg)
		{
			this->path = p;
		}

		u8 *image;
		int width, height, numChannels, bufferId, layerId;
		bool loadedToGpu = true;
	};
}

#endif
