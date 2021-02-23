#include "vbModelReaderTest.h"
#include "vbModelReader.h"
#include "skeleton.h"
#include "animationController.h"

using namespace std;

namespace vb01{
	VbModelReaderTest::VbModelReaderTest(){}

	VbModelReaderTest::~VbModelReaderTest(){}

	void VbModelReaderTest::setUp(){
		rootNode = Root::getSingleton()->getRootNode();
		modelReader = new VbModelReader();
		setupSkeleton();
		setupMesh();
	}

	void VbModelReaderTest::tearDown(){
		delete modelReader;
	}

	void VbModelReaderTest::setupSkeleton(){
		model = new Model();
		rootNode->attachChild(model);
		modelReader->model = model;

		bones = vector<vbReader::Bone>({
				Bone("bipod", "-", Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0), "", 0),
				Bone("bipod.L", "bipod", Vector3(1, 0, 2), Vector3(1, 0, 0), Vector3(0, 0, -1), "", 0),
				Bone("bipod.R", "bipod", Vector3(-1, 0, 2), Vector3(-1, 0, 0), Vector3(0, 0, -1), "", 0),
				Bone("opening", "-", Vector3(0, 0, 2), Vector3(-1, 0, 0), Vector3(0, 0, -1), "", 0)
		});
		skeletonParent = "-";

		int numBones = bones.size();

		skeletonData.push_back("ARMATURE: MG42");
		skeletonData.push_back("pos: 0.0 0.0 0.0");
		skeletonData.push_back("rot: 1.0 0.0 0.0 -0.0");
		skeletonData.push_back("scale: 1.0 1.0 1.0");
		skeletonData.push_back("parent: " + skeletonParent);
		skeletonData.push_back("bones: " + to_string(numBones) + " ");
		for(vbReader::Bone b : bones){
			string headStr = to_string(b.head.x) + " " + to_string(b.head.y) + " " + to_string(b.head.z) + " ";
			string xAxisStr = to_string(b.xAxis.x) + " " + to_string(b.xAxis.y) + " " + to_string(b.xAxis.z) + " ";
			string yAxisStr = to_string(b.yAxis.x) + " " + to_string(b.yAxis.y) + " " + to_string(b.yAxis.z) + " ";
			skeletonData.push_back(b.name + ": " + b.parent + " " + headStr + xAxisStr + yAxisStr + b.ikTarget + " " + to_string(b.ikChainLength) + " ");
		}
		skeletonData.push_back("animations: 2");
		skeletonData.push_back("animation: Action");
		skeletonData.push_back("groups: 2 ");
		skeletonData.push_back("bipod.L 7 pos_x 2 pos_y 2 pos_z 2 rot_w 2 rot_x 2 rot_y 2 rot_z 2 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("bipod.R 6 pos_x 2 pos_y 2 pos_z 2 rot_w 2 rot_x 2 rot_y 2 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("10 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("10 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("10 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("10 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("10 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("10 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("animation: open");
		skeletonData.push_back("groups: 1 ");
		skeletonData.push_back("opening 1 rot_w 2 rot_x 1");
		skeletonData.push_back("1 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 10 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
		skeletonData.push_back("0 1 2 AUTO_CLAMPED 1 1 AUTO_CLAMPED 1 1 ");
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
		meshData.push_back("shapeKeys: 0");
		meshData.push_back("Key 1: 0 1");
		for(int i = 0; i < vertPos.size(); i++){
			Vector3 pos = vertPos[i];
			string posString = to_string(pos.x) + " " + to_string(pos.y) + " " + to_string(pos.z) + " ";
			meshData.push_back(posString);
		}
		meshData.push_back("animations: 0");
	}

	void VbModelReaderTest::testReadSkeletons(){
		Skeleton *skeleton = modelReader->readSkeleton(skeletonData);
		int numBones = skeleton->getNumBones();
		for(int i = 0; i < numBones; i++){
			CPPUNIT_ASSERT(skeleton->getBone(i)->getName() == bones[i].name);

			vb01::Bone *parent = (vb01::Bone*)skeleton->getBone(i)->getParent();
			if(parent->getName() != "")
				CPPUNIT_ASSERT(parent->getName() == bones[i].parent);
			else
				CPPUNIT_ASSERT(bones[i].parent == "-");
		}
		AnimationController *controller = skeleton->getAnimationController();
		string animNames[]{"Action", "open"};
		CPPUNIT_ASSERT(controller->getAnimation(animNames[0]));
		CPPUNIT_ASSERT(controller->getAnimation(animNames[1]));
		Animation *animation = controller->getAnimation(animNames[0]);

		const vector<KeyframeChannel> &keyframeChannels = animation->getKeyframeChannels();
	   	vector<KeyframeChannel> leftBipodChannels, rightBipodChannels;
		for(KeyframeChannel ch : keyframeChannels){
			vb01::Bone *bone = (vb01::Bone*)ch.animatable;
			if(bone->getName() == "bipod.L")
				leftBipodChannels.push_back(ch);
			else if(bone->getName() == "bipod.R")
				rightBipodChannels.push_back(ch);
		}
		CPPUNIT_ASSERT(!leftBipodChannels.empty());
		CPPUNIT_ASSERT(!rightBipodChannels.empty());
		for(KeyframeChannel keyframeChannel : leftBipodChannels)
			CPPUNIT_ASSERT(keyframeChannel.keyframes.size() == 2);
		for(KeyframeChannel keyframeChannel : rightBipodChannels)
			CPPUNIT_ASSERT(keyframeChannel.keyframes.size() == 2);
		
		Keyframe lastRightBipodKeyframe = animation->getKeyframeChannel(skeleton->getBone("bipod.R"), KeyframeChannelType::ROT_Y)->keyframes[1];
		CPPUNIT_ASSERT(lastRightBipodKeyframe.value == 10);
		CPPUNIT_ASSERT(lastRightBipodKeyframe.frame == 10);
		CPPUNIT_ASSERT(lastRightBipodKeyframe.interpolation == KeyframeInterpolation::BEZIER);
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
