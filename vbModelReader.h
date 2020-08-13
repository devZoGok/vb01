#ifndef VB_MODEL_READER
#define VB_MODEL_READER

#include"modelReader.h"
#include<string>
#include<vector>

namespace vb01{
	class Model;
	class Node;
	class Mesh;
	class Skeleton;

	class VbModelReader : public ModelReader{
		public:
			VbModelReader(Model*, std::string);
			~VbModelReader();
		private:
			void readSkeletons(int, int);
			void readMeshes(int, int);
			void readLights(int, int);

			std::vector<std::string> relationships;
			std::vector<Skeleton*> skeletons;
			std::vector<Mesh*> meshes;
			std::vector<Node*> nodes;
	};
}

#endif

