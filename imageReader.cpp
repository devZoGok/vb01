#include "imageReader.h"

#include "stb_image.h"

using namespace std;

namespace vb01{
		ImageReader *imageReader = nullptr;

		ImageReader* ImageReader::getSingleton(){
				if(!imageReader)
						imageReader = new ImageReader();

				return imageReader;
		}

		ImageAsset* ImageReader::readImage(string path, bool flip){
			stbi_set_flip_vertically_on_load(flip);
			u8 *data = stbi_load(path.c_str(), nullptr, nullptr, nullptr, 0);
			return new ImageAsset(path, data);
		}
}
