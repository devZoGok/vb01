#ifndef VB_MODEL_READER
#define VB_MODEL_READER

#include "modelReader.h"
#include "util.h"
#include "mesh.h"

#include <vector>
#include <iterator>
#include <map>

namespace vb01{
	class Model;
	class Node;
	class Skeleton;
	class Driver;

	class VbModelReader : public ModelReader{
		public:
			VbModelReader(){}
			VbModelReader(Model*, std::string);
			~VbModelReader();
		private:
			void getObjectBounds(std::map<int, int>&, std::map<int, int>&, std::map<int, int>&);
			void connectNodes();
			void setupDrivers();
			void createBones(Skeleton*, std::vector<std::string>&);
			std::vector<KeyframeChannel> readKeyframeChannels(Animatable*, std::vector<std::string>&, int&);
			void readAnimations(Animatable*, AnimationController*, std::vector<std::string>&);
			Skeleton* readSkeleton(std::vector<std::string>&);
			void readVertices(std::vector<Vector3>&, std::vector<Vector3>&, std::vector<float>&, std::vector<std::string>&, int);
			void readFaces(std::vector<Vector3>&, std::vector<Vector3>&, std::vector<u32>&, std::vector<float>&, std::vector<std::string>&, int, Mesh::Vertex*, u32*);
			void readVertexGroups(std::string*, std::vector<std::string>&, int);
			void readShapeKeys(Mesh::Vertex*, std::vector<u32>&, std::vector<std::string>&, Mesh::ShapeKey*, int, int);
			void readDrivers(std::vector<std::string>&, std::string);
			void buildMesh(std::vector<std::string>&);
			Mesh* readMeshes(std::vector<std::string>&);
			void readLights(std::vector<std::string>&);

			std::vector<Driver*> drivers;
			std::map<std::string, std::string> relationships;
			std::vector<std::string*> driverSetups;
			Skeleton *currentSkeleton = nullptr;
			std::vector<Skeleton*> skeletons;
			std::vector<Mesh*> meshes;
			std::vector<Node*> nodes;
			std::string currentAnim = "";

			friend class VbModelReaderTest;
	};
}

#endif

