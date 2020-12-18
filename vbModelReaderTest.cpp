#include "vbModelReaderTest.h"
#include "vbModelReader.h"

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
	}

	void VbModelReaderTest::setupMesh(){
		meshData.push_back("MESH: mg42");
		meshData.push_back("pos: 0.0 0.0 0.0");
		meshData.push_back("rot: 1.0 0.0 0.0 -0.0");
		meshData.push_back("scale: 1.0 1.0 1.0");
		meshData.push_back("parent: -");
		meshData.push_back("skeleton: MG42");
		meshData.push_back("numElements: 3 1 2 0 ");
		meshData.push_back("groups:");
		meshData.push_back("clip");
		meshData.push_back("trigger");
		meshData.push_back("vertices:");
		meshData.push_back("0 1 2 3 4 5 1 0.5 ");
		meshData.push_back("1 1 2 3 4 5 1 0.5 ");
		meshData.push_back("2 1 2 3 4 5 1 0.5 ");
		meshData.push_back("faces:");
		meshData.push_back("0 1 1 1 2 3 3 2 1");
		meshData.push_back("1 0 0 1 2 3 3 2 1");
		meshData.push_back("2 1 0 1 2 3 3 2 1");
	}

	void VbModelReaderTest::testReadSkeletons(){
	}

	void VbModelReaderTest::testReadMeshes(){
		modelReader->readMeshes(meshData);
		Mesh *mesh = modelReader->meshes[0];
		CPPUNIT_ASSERT(mesh->getName() == "mg42");
	}
}
