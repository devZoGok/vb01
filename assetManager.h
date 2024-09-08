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
			void editAsset(std::string, Asset&);
			Asset* getAsset(std::string);
			inline Asset* getAsset(int i){return assets[i];}
			inline std::vector<Asset*> getAssets(){return assets;}
			inline int getNumAssets(){return assets.size();}
		private:
			AssetManager(){}
			void readDir(std::string, std::vector<std::string>&, bool);

			std::vector<Asset*> assets;
	};
}

#endif
