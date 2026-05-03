#ifndef ASSIMP_MODEL_READER_H
#define ASSIMP_MODEL_READER_H

#include "abstractAssetReader.h"

class aiNode;
class aiMesh;
class aiScene;

namespace vb01{
		struct Asset;
		class Node;

	class AssimpModelReader : public AbstractAssetReader{
		public:
			static AssimpModelReader* getSingleton();
			Asset* readAsset(std::string);
		private:
			AssimpModelReader(){}
			void processNode(aiNode*, const aiScene*, Node*);
			void processMesh(aiMesh*, const aiScene*, Node*);
			void processMaterial();
	};
}

#endif
