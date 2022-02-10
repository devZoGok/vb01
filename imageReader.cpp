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

		Asset* ImageReader::readAsset(string path){
			stbi_set_flip_vertically_on_load(flip);
			int width, height, numChannels;
			u8 *data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
			return new ImageAsset(path, data, width, height);
		}
}
