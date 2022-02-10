#include "assetManager.h"
#include "imageReader.h"
#include "util.h"

#include <algorithm>

using namespace std;

namespace vb01{
		AssetManager *assetManager = nullptr;

		AssetManager* AssetManager::getSingleton(){
				if(!assetManager)
						assetManager = new AssetManager();

				return assetManager;
		}

		void AssetManager::load(string path){
				if(getAsset(path))
						return;

				string format = path.substr(path.find_last_of(".") + 1, string::npos);
				vector<string> imageFormats = vector<string>{"png", "jpg", "jpeg"};

				if(*find(imageFormats.begin(), imageFormats.end(), format) == format)
						assets.push_back(ImageReader::getSingleton()->readImage(path, false));
		}

		Asset* AssetManager::getAsset(string path){
				for(Asset *a : assets)
						if(a->path == path)
								return a;
				
				return nullptr;
		}
}
