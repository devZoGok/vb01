#ifndef VB_MODEL_READER
#define VB_MODEL_READER

#include"modelReader.h"
#include"util.h"
#include"mesh.h"

#include<string>
#include<vector>
#include<iterator>
#include<map>

namespace vb01{
	class Model;
	class Node;
	class Skeleton;

	class VbModelReader : public ModelReader{
		public:
			VbModelReader(){}
			VbModelReader(Model*, std::string);
			~VbModelReader();
		private:
			void getObjectBounds(std::map<int, int>&, std::map<int, int>&, std::map<int, int>&);
			void connectNodes();

			void createBones(Skeleton*, std::vector<std::string>&);
			void readAnimations(Skeleton*, std::vector<std::string>&);
			void readKeyframesGroups(Skeleton*, std::vector<std::string>&);
			void readKeyframes(Skeleton*, std::vector<std::string>&);
			void readSkeleton(std::vector<std::string>&);
			void readVertices(std::vector<Vector3>&, std::vector<Vector3>&, std::vector<float>&, std::vector<std::string>&, int);
			void readFaces(std::vector<Vector3>&, std::vector<Vector3>&, std::vector<float>&, std::vector<std::string>&, int, Mesh::Vertex*, u32*);
			void readVertexGroups(std::string*, std::vector<std::string>&, int);
			void readShapeKeys(int, int);
			void readMeshes(std::vector<std::string>&);
			void readLights(std::vector<std::string>&);

			std::vector<std::string> relationships;
			std::vector<Skeleton*> skeletons;
			std::vector<Mesh*> meshes;
			std::vector<Node*> nodes;

			friend class VbModelReaderTest;
	};
}

#endif

