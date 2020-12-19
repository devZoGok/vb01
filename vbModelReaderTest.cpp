#include "vbModelReaderTest.h"
#include "vbModelReader.h"
#include "skeleton.h"

using namespace std;

namespace vb01{
	VbModelReaderTest::VbModelReaderTest(){}

	VbModelReaderTest::~VbModelReaderTest(){}

	void VbModelReaderTest::setUp(){
		modelReader = new VbModelReader();
		setupSkeleton();
		setupMesh();
	}

	void VbModelReaderTest::tearDown(){
		delete modelReader;
	}

	void VbModelReaderTest::setupSkeleton(){
		bones = vector<vbReader::Bone>({
				Bone("", "", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), "", 0)
		});
		skeletonParent = "-";

		animations = vector<Animation>({
		});

		int numBones = bones.size();

		skeletonData.push_back("ARMATURE: MG42");
		skeletonData.push_back("pos: 0.0 0.0 0.0");
		skeletonData.push_back("rot: 1.0 0.0 0.0 -0.0");
		skeletonData.push_back("scale: 1.0 1.0 1.0");
		skeletonData.push_back("parent: " + skeletonParent);
		skeletonData.push_back("bones: " + to_string(numBones) + " 1 ");
		for(vbReader::Bone b : bones){
			string headStr = to_string(b.head.x) + " " + to_string(b.head.y) + " " + to_string(b.head.z) + " ";
			string xAxisStr = to_string(b.xAxis.x) + " " + to_string(b.xAxis.y) + " " + to_string(b.xAxis.z) + " ";
			string yAxisStr = to_string(b.yAxis.x) + " " + to_string(b.yAxis.y) + " " + to_string(b.yAxis.z) + " ";
			skeletonData.push_back(b.name + ": " + " " + b.parent + " " + headStr + xAxisStr + yAxisStr + b.ikTarget + " " + to_string(b.ikChainLength) + " ");
		}
		skeletonData.push_back("animations: 1");
		skeletonData.push_back("animation: Action 3 bipod bipod.R bipod.L ");
		skeletonData.push_back("bipod: 3 pos_x 3 pos_y 3 pos_z 3 rot_w 3 rot_x 3 rot_y 3 rot_z ");
	}

	void VbModelReaderTest::setupMesh(){
		meshName = "mg42";
		meshParent = "-";
		skeletonName = "MG42";

		vertPos = vector<Vector3>({Vector3(0, 1, 2), Vector3(1, 1, 2), Vector3(2, 1, 2)});
		vertNorm = vector<Vector3>({Vector3(3, 4, 5), Vector3(3, 4, 5), Vector3(3, 4, 5)});
		vertUv = vector<Vector2>({Vector2(1, 1), Vector2(1, 0), Vector2(0, 0)});
		vertexGroups = vector<string>({"clip", "trigger"});
		vertTan = vector<Vector3>({Vector3(1, 2, 3), Vector3(1, 2, 3), Vector3(1, 2, 3)});
		vertBiTan = vector<Vector3>({Vector3(3, 2, 1), Vector3(3, 2, 1), Vector3(3, 2, 1)});
		vertWeights = vector<vector<float>>({
				vector<float>({1, 0.5}),
				vector<float>({1, 0.5}),
				vector<float>({1, 0.5})
				});

		int numVerts = vertPos.size();
		int numFaces = numVerts / 3;
		int numVertexGroups = vertexGroups.size();
		int numShapeKeys = 0;

		meshData.push_back("MESH: " + meshName);
		meshData.push_back("pos: 0.0 0.0 0.0");
		meshData.push_back("rot: 1.0 0.0 0.0 -0.0");
		meshData.push_back("scale: 1.0 1.0 1.0");
		meshData.push_back("parent: " + meshParent);
		meshData.push_back("skeleton: " + skeletonName);
		meshData.push_back("numElements: " + to_string(numVerts) + " " + to_string(numFaces) + " " + to_string(numVertexGroups) + " " + to_string(numShapeKeys) + " ");
		meshData.push_back("groups:");
		for(string group : vertexGroups)
			meshData.push_back(group);
		meshData.push_back("vertices:");
		for(int i = 0; i < vertPos.size(); i++){
			Vector3 pos = vertPos[i];
			Vector3 norm = vertNorm[i];
			string posString = to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z) + " ";
			string normString = to_string(norm.x) + " " + to_string(norm.y) + " " + to_string(norm.z) + " ";
			string weightsString = "";
			for(int j = 0; j < numVertexGroups; j++)
				weightsString += to_string(vertWeights[i][j]) + " ";

			meshData.push_back(posString + normString + weightsString);
		}
		meshData.push_back("faces:");
		for(int i = 0; i < vertPos.size(); i++){
			Vector2 uv = vertUv[i];
			Vector3 tan = vertTan[i];
			Vector3 biTan = vertBiTan[i];
			string uvString = to_string(uv.x) + " " + to_string(uv.y) + " ";
			string tanString = to_string(tan.x) + " " + to_string(tan.y) + " " + to_string(tan.z) + " ";
			string biTanString = to_string(biTan.x) + " " + to_string(biTan.y) + " " + to_string(biTan.z) + " ";

			meshData.push_back(to_string(i) + " " + uvString + tanString + biTanString);
		}
	}

	void VbModelReaderTest::testReadSkeletons(){
		//Skeleton *skeleton = modelReader->readSkeleton(skeletonData);
	}

	void VbModelReaderTest::testReadMeshes(){
		Mesh *mesh = modelReader->readMeshes(meshData);
		Mesh::Vertex *vertices = mesh->getVerts();
		u32 *indices = mesh->getIndices();
		Skeleton *skeleton = mesh->getSkeleton();

		CPPUNIT_ASSERT(mesh->getName() == meshName);
		if(skeleton)
			CPPUNIT_ASSERT(mesh->getSkeleton()->getName() == skeletonName);
		else
			CPPUNIT_ASSERT(!mesh->getSkeleton());

		for(int i = 0; i < mesh->getNumVerts(); i++){
			Mesh::Vertex vert = vertices[i];
			Vector3 pos = vertPos[i], norm = vertNorm[i], tan = vertTan[i], biTan = vertBiTan[i];
			Vector2 uv = vertUv[i];

			CPPUNIT_ASSERT(vert.pos == Vector3(pos.x, pos.z, -pos.y));
			CPPUNIT_ASSERT(vert.norm == Vector3(norm.x, norm.z, -norm.y));
			CPPUNIT_ASSERT(vert.tan == Vector3(tan.x, tan.z, -tan.y));
			CPPUNIT_ASSERT(vert.biTan == Vector3(biTan.x, biTan.z, -biTan.y));
			CPPUNIT_ASSERT(vert.uv == uv);
			CPPUNIT_ASSERT(indices[i] == i);
		}

		for(int i = 0; i < mesh->getNumVertexGroups(); i++){
			CPPUNIT_ASSERT(mesh->getVertexGroups()[i] == vertexGroups[i]);
		}
	}
}
