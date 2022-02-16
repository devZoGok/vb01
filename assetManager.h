#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "asset.h"

#include <vector>

namespace vb01{
		class AbstractAssetReader;

		class AssetManager{
				public:
						static AssetManager* getSingleton();
						void load(std::string);
						Asset* getAsset(std::string);
				private:
						AssetManager(){}

						std::vector<Asset*> assets;
		};
}

#endif
