#ifndef VB_MODEL_READER
#define VB_MODEL_READER

#include"modelReader.h"
#include"util.h"
#include"mesh.h"
#include<string>
#include<vector>

namespace vb01{
	class Model;
	class Node;
	class Skeleton;

	class VbModelReader : public ModelReader{
		public:
			VbModelReader(Model*, std::string);
			~VbModelReader();
		private:
			void readBones(std::vector<std::string>&, Skeleton*, int, int);
			void readAnimations(std::vector<std::string>&, Skeleton*, int, int);
			void readSkeletons(int, int);
			void readVertices(std::vector<std::string>&, std::vector<Vector3>&, std::vector<Vector3>&, std::vector<float>&, int, int, int);
			void readFaces(std::vector<std::string>&, std::vector<Vector3>&, std::vector<Vector3>&, std::vector<float>&, int, int, int, int, Mesh::Vertex*, u32*);
			void readMeshes(int, int);
			void readLights(int, int);

			std::vector<std::string> relationships;
			std::vector<Skeleton*> skeletons;
			std::vector<Mesh*> meshes;
			std::vector<Node*> nodes;
	};
}

#endif

