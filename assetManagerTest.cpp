#include "assetManagerTest.h"
#include "assetManager.h"

#include <string>
#include <vector>

namespace vb01{
	using namespace std;

	void AssetManagerTest::setUp(){
		assetManager = AssetManager::getSingleton();

		string basePath = "../tests/";
		recursiveLoadPath = basePath + "recursiveLoad/";
		fileLoadPath = basePath + "fileLoad/";
	}

	void AssetManagerTest::tearDown(){
	}

	void AssetManagerTest::testLoad(){
		vector<string> recursiveLoadFiles = vector<string>{"folderA/a.png", "folderB/b.png"};
		assetManager->load(recursiveLoadPath, true);

		for(string file : recursiveLoadFiles){
			Asset *asset = assetManager->getAsset(recursiveLoadPath + file);
			CPPUNIT_ASSERT(asset != nullptr);
		}

		string file = fileLoadPath + "a.png";
		assetManager->load(file);
		CPPUNIT_ASSERT(assetManager->getAsset(file) != nullptr);

		assetManager->load(file);
		vector<Asset*> assets = assetManager->getAssets();
		int numSameAssets = 0;

		for(int i = 0; i < assets.size(); i++)
			if(assets[i]->path == file)
				numSameAssets++;

		CPPUNIT_ASSERT(numSameAssets == 1);

		string nonReadableFile = fileLoadPath + "b.abc";
		assetManager->load(nonReadableFile);
		CPPUNIT_ASSERT(!assetManager->getAsset(nonReadableFile));
	}
}
