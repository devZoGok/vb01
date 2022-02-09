#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include "util.h"
#include "imageAsset.h"

namespace vb01{
		class ImageReader{
				public:
						static ImageReader* getSingleton();
						ImageAsset* readImage(std::string, bool);
				private:
						ImageReader(){}
		};
}

#endif
