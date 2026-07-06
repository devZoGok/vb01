#include "imageReader.h"
#include "root.h"

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
		int width, height, numChannels, layerId;
		u8 *data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

		if(loadToGpu){
			Root *root = Root::getSingleton();
			u32 *texBuffer = (sceneImage ? root->getMeshTextureBuffer() : root->getGuiTextureBuffer());
			root->initTextureDataOnGpu(texBuffer, data, width, height, layerId);
		}

		return new ImageAsset(path, data, width, height, numChannels, layerId);
	}
}
