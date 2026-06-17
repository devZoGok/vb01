#include "assetManager.h"
#include "abstractAssetReader.h"
#include "util.h"
#include "root.h"
#include "node.h"
#include "mesh.h"
#include "material.h"
#include "imageReader.h"
#include "fontReader.h"
#include "shaderReader.h"
#include "xmlModelReader.h"

#include <tinydir.h>

#include <algorithm>
#include <iostream>

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

		if(tinydir_open_sorted(&dir, path.c_str()) == -1){
			contents.push_back(path);
			return;
		}

		for(int i = 0; i < dir.n_files; i++){
			tinydir_file file;
			tinydir_readfile_n(&dir, &file, i);

			if(file.name[0] == '.')
				continue;

			if(file.is_dir && recursive)
				readDir(path + file.name + "/", contents, recursive);
			else if(!file.is_dir)
				contents.push_back(path + file.name);
		}

		tinydir_close(&dir);
	}

	void AssetManager::load(string path, bool recursive){
		vector<string> files;
		readDir(path, files, recursive);

		for(string file : files){
			if(getAsset(file))
				continue;

			string format = file.substr(file.find_last_of(".") + 1, string::npos);
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

			vector<string> shaderFormats = vector<string>{"vert", "frag", "geo"};

			if(find(shaderFormats.begin(), shaderFormats.end(), format) != shaderFormats.end())
				assetReader = ShaderReader::getSingleton();

			if(assetReader)
				assets.push_back(assetReader->readAsset(file));
			else
				cout << "No asset reader for " << file << endl;
		}
	}

	Asset* AssetManager::getAsset(string path){
		for(Asset *a : assets)
			if(a->path == path)
				return a;
		
		return nullptr;
	}

	void AssetManager::editAsset(string path, Asset &newAsset){
		ShaderAsset *oldAsset = (ShaderAsset*)getAsset(path);

		if(oldAsset){
			oldAsset->shaderString = ((ShaderAsset&)newAsset).shaderString;

			Root *root = Root::getSingleton();
			vector<Node*> descendants;
			root->getRootNode()->getDescendants(descendants);
			root->getGuiNode()->getDescendants(descendants);

			for(Node *desc : descendants){
				vector<Mesh*> meshes = desc->getMeshes();

				for(Mesh *mesh : meshes){
					Material *mat = mesh->getMaterial();

					if(mat)
						mat->getShader()->loadShaders();
				}
			}
		}
	}
}
