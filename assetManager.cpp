#include "assetManager.h"
#include "abstractAssetReader.h"
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

		void AssetManager::load(AbstractAssetReader *assetReader, string path){
				if(getAsset(path))
						return;

				assets.push_back(assetReader->readAsset(path));
		}

		Asset* AssetManager::getAsset(string path){
				for(Asset *a : assets)
						if(a->path == path)
								return a;
				
				return nullptr;
		}
}
