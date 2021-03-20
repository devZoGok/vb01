#ifndef ASSIMP_MODEL_READER_H
#define ASSIMP_MODEL_READER_H

#include "modelReader.h"

class aiNode;
class aiMesh;
class aiScene;

namespace vb01{
	class AssimpModelReader : public ModelReader{
		public:
			AssimpModelReader(Model*, std::string);
		private:
			void processNode(aiNode*, const aiScene*, Node*);
			void processMesh(aiMesh*, const aiScene*, Node*);
			void processMaterial();
	};
}

#endif
