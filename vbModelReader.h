#ifndef MODEL_READER
#define MODEL_READER

#include<string>
#include"modelReader.h"

namespace vb01{
	class Model;
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

			std::string path;
			std::vector<std::string> relationships;
			std::vector<Skeleton*> skeletons;
			std::vector<Mesh*> meshes;
			std::vector<Node*> nodes;
	};
}

#endif

