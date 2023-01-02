#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "asset.h"

#include <vector>

namespace vb01{
	class AbstractAssetReader;

	class AssetManager{
		public:
			static AssetManager* getSingleton();
			void load(std::string, bool = false);
			Asset* getAsset(std::string);
			inline std::vector<Asset*> getAssets(){return assets;}
		private:
			AssetManager(){}
			void readDir(std::string, std::vector<std::string>&, bool);

			std::vector<Asset*> assets;
	};
}

#endif
