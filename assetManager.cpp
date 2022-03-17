#include "assetManager.h"
#include "abstractAssetReader.h"
#include "util.h"
#include "imageReader.h"
#include "fontReader.h"
#include "xmlModelReader.h"

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
				AbstractAssetReader *assetReader = nullptr;
				vector<string> imageFormats = vector<string>{"png", "jpeg", "jpg"};

				if(find(imageFormats.begin(), imageFormats.end(), format) != imageFormats.end())
						assetReader = ImageReader::getSingleton();

				vector<string> fontFormats = vector<string>{"ttf"};

				if(find(fontFormats.begin(), fontFormats.end(), format) != fontFormats.end())
						assetReader = FontReader::getSingleton();

				vector<string> modelFormats = vector<string>{"xml"};

				if(find(modelFormats.begin(), modelFormats.end(), format) != modelFormats.end())
						assetReader = XmlModelReader::getSingleton();

				assets.push_back(assetReader->readAsset(path));
		}

		Asset* AssetManager::getAsset(string path){
				for(Asset *a : assets)
						if(a->path == path)
								return a;
				
				return nullptr;
		}
}
