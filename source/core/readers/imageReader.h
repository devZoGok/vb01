#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include "util.h"
#include "imageAsset.h"
#include "abstractAssetReader.h"

namespace vb01{
		class ImageReader : public AbstractAssetReader{
				public:
						static ImageReader* getSingleton();
						inline void setFlip(bool flip){this->flip = flip;}
						inline bool isFlip(){return flip;}
						Asset* readAsset(std::string);
				private:
						ImageReader(){}

						bool flip = true;
		};
}

#endif
