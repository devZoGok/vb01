#include "assetManager.h"
#include "abstractAssetReader.h"
#include "util.h"
#include "imageReader.h"
#include "fontReader.h"
#include "xmlModelReader.h"

#include <tinydir.h>

#include <algorithm>

using namespace std;

namespace vb01{
	AssetManager *assetManager = nullptr;

	AssetManager* AssetManager::getSingleton(){
		if(!assetManager)
			assetManager = new AssetManager();

		return assetManager;
	}

	void AssetManager::readDir(string path, vector<string> &contents, bool recursive){
		tinydir_dir dir;
		tinydir_open_sorted(&dir, path.c_str());

		for(int i = 0; i < dir.n_files; i++){
			tinydir_file file;
			tinydir_readfile_n(&dir, &file, i);

			if(file.is_dir && recursive)
				readDir(path + file.name, contents, recursive);
			else if(!file.is_dir)
				contents.push_back(file.name);
		}

		tinydir_close(&dir);
	}

	void AssetManager::load(string path, bool recursive){
		if(getAsset(path))
			return;

		vector<string> files;
		readDir(path, files, recursive);

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
