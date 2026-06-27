#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "asset.h"

#include <vector>

namespace vb01{
	class AbstractAssetReader;

	class AssetManager{
		public:
			static AssetManager* getSingleton();
			void readDir(std::string, std::vector<std::string>&, bool);
			void load(std::string, bool = false);
			void editAsset(std::string, Asset&);
			Asset* getAsset(std::string);
			std::vector<Asset*> getAssets(std::vector<std::string>);
			inline std::vector<std::string> getImageFormats(){return imageFormats;}
			inline std::vector<std::string> getShaderFormats(){return shaderFormats;}
			inline std::vector<std::string> getModelFormats(){return modelFormats;}
			inline std::vector<std::string> getFontFormats(){return fontFormats;}
			inline Asset* getAsset(int i){return assets[i];}
			inline std::vector<Asset*> getAssets(){return assets;}
			inline int getNumAssets(){return assets.size();}
		private:
			AssetManager(){}

			std::vector<Asset*> assets;
			const std::vector<std::string> imageFormats = std::vector<std::string>{"png", "jpeg", "jpg"};
			const std::vector<std::string> shaderFormats = std::vector<std::string>{"vert", "frag", "geo"};
			const std::vector<std::string> modelFormats = std::vector<std::string>{"xml"};
			const std::vector<std::string> fontFormats = std::vector<std::string>{"ttf"};
	};
}

#endif
