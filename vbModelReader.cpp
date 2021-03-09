#include"vbModelReader.h"
#include"util.h"
#include"vector.h"
#include"mesh.h"
#include"skeleton.h"
#include"bone.h"
#include"model.h"
#include"animation.h"
#include"animationController.h"

#include<vector>
#include<algorithm>
#include<fstream>

using namespace std;

namespace vb01{
	VbModelReader::VbModelReader(Model *model, string path) : ModelReader(model, path){
		this->path = path;
		this->model = model;

		map<int, int> meshBracketIds, skeletonBracketIds, lightBracketIds;	
		getObjectBounds(meshBracketIds, skeletonBracketIds, lightBracketIds);

		map<int, int>::iterator it;
		vector<string> data;
		for(it = skeletonBracketIds.begin(); it != skeletonBracketIds.end(); ++it){
			int key = it->first;
			readFile(path, data, key + 1, skeletonBracketIds[key]);
			skeletons.push_back(readSkeleton(data));
			data.clear();
		}
		for(it = meshBracketIds.begin(); it != meshBracketIds.end(); ++it){
			int key = it->first;
			readFile(path, data, key + 1, meshBracketIds[key]);
			buildMesh(data);
			data.clear();
		}
		for(it = lightBracketIds.begin(); it != lightBracketIds.end(); ++it){
			int key = it->first;
			readFile(path, data, key + 1, lightBracketIds[key]);
			readLights(data);
			data.clear();
		}

		connectNodes();
		setupDrivers();
	}

	VbModelReader::~VbModelReader(){
	}

	void VbModelReader::getObjectBounds(map<int, int> &meshBracketIds, map<int, int> &skeletonBracketIds, map<int, int> &lightBracketIds){
		const string meshType = "MESH", skeletonType = "ARMATURE", lightType = "LIGHT";

		int lineIndex = 0, leftBracket = -1, rightBracket = -1;
		string line, type;
		ifstream file(path);

		while(getline(file, line)){
			if(line[0] == '{'){
				leftBracket = lineIndex;

				getline(file, line);
				lineIndex++;

				int colonId = getCharId(line, ':');
				type = line.substr(0, colonId);
			}
			else if(line[0] == '}')
				rightBracket = lineIndex;

			if(leftBracket != -1 && rightBracket != -1){
				if(type == meshType)
					meshBracketIds.emplace(leftBracket, rightBracket);
				else if(type == skeletonType)
					skeletonBracketIds.emplace(leftBracket, rightBracket);
				else if(type == lightType)
					lightBracketIds.emplace(leftBracket, rightBracket);

				leftBracket = -1, rightBracket = -1;
			}

			lineIndex++;
		}

		file.close();
	}

	void VbModelReader::connectNodes(){
		for(map<string, string>::iterator it = relationships.begin(); it != relationships.end(); it++){
			Node *child = nullptr, *parent = nullptr;
			string childName = it->first, parentName = relationships[childName];
			for(Node *node : nodes){
				if(node->getName() == childName)
					child = node;
				else if(node->getName() == parentName)
					parent = node;
			}
			if(parent)
				parent->attachChild(child);
		}
		for(Node *node : nodes)
			if(!node->getParent())
				model->attachChild(node);
	}

	void VbModelReader::setupDrivers(){
		int i = 0;
		for(string *data : driverSetups){
			Skeleton *drivingSkeleton = nullptr, *drivenSkeleton = nullptr;
			Mesh *drivenMesh = nullptr;

			for(Skeleton *sk : skeletons){
				if(data[0] == sk->getName())
					drivingSkeleton = sk;
				if(data[2] == sk->getName())
					drivenSkeleton = sk;
			}
			for(Mesh *m : meshes)
				if(data[2] == m->getName())
					drivenMesh = m;

			Animatable *animatable = nullptr;
			if(drivenSkeleton){
				for(Bone *bone : drivenSkeleton->getBones())
					if(bone->getName() == data[3])
						animatable = bone;
			}
			else if(drivenMesh){
				int numShapeKeys = drivenMesh->getNumShapeKeys();
				for(int i = 0; i < numShapeKeys; i++){
					Mesh::ShapeKey &shapeKey = drivenMesh->getShapeKey(i);
					if(shapeKey.name == data[3])
						animatable = &shapeKey;
				}
			}
			else{
				for(Node *node : nodes)
					if(node->getName() == data[2])
						animatable = node;
			}

			if(drivingSkeleton){
				for(Bone *bone : drivingSkeleton->getBones())
					if(bone->getName() == data[1])
						bone->addDriver(drivers[i]);
			}
			else{
				for(Node *node : nodes)
					if(node->getName() == data[0])
						node->addDriver(drivers[i]);
			}

			KeyframeChannel &keyframeChannel = drivers[i]->getKeyframeChannel();
			keyframeChannel.animatable = animatable;

			i++;
		}

		for(string *data : driverSetups)
			delete[] data;
	}

	void VbModelReader::createBones(Skeleton *skeleton, vector<string> &skeletonData){
		map<string, string> ikRelationships;

		for(string line : skeletonData){
			int colonId = getCharId(line, ':');
			string preColon = line.substr(0, colonId);
			string postColon = line.substr(colonId + 2);

			int numData = 13;
			string data[numData];
			getLineData(postColon, data, numData);

			float length = atof(data[1].c_str());
			Vector3 head = Vector3(atof(data[2].c_str()), atof(data[3].c_str()), atof(data[4].c_str()));
			Vector3 xAxis = Vector3(atof(data[5].c_str()), atof(data[6].c_str()), atof(data[7].c_str())).norm();
			Vector3 yAxis = Vector3(atof(data[8].c_str()), atof(data[9].c_str()), atof(data[10].c_str())).norm();

			float eps = pow(10, -2);
			if(fabs(xAxis.x) < eps)
				xAxis.x = 0;
			if(fabs(xAxis.y) < eps)
				xAxis.y = 0;
			if(fabs(xAxis.z) < eps)
				xAxis.z = 0;
			if(fabs(yAxis.x) < eps)
				yAxis.x = 0;
			if(fabs(yAxis.y) < eps)
				yAxis.y = 0;
			if(fabs(yAxis.z) < eps)
				yAxis.z = 0;

			string ikTarget = data[11].c_str();
			int ikChainLength = atoi(data[12].c_str());
			Vector3 zAxis = xAxis.cross(yAxis).norm();
			Vector3 pos = head;

			string parName = string(data[0].c_str());
			Node *parent = skeleton->getBone(parName);
			if(!parent){
				parent = model;
				swap(pos.y, pos.z);
				pos.z = -pos.z;
				swap(yAxis.y, yAxis.z);
				yAxis.z = -yAxis.z;
				swap(zAxis.y, zAxis.z);
				zAxis.z = -zAxis.z;
			}
			else
				pos = pos + Vector3(0, ((Bone*)parent)->getLength(), 0);

			Bone *bone = new Bone(preColon, length, pos);
			bone->setIkChainLength(ikChainLength);
			skeleton->addBone(bone, (Bone*)parent);
			bone->lookAt(zAxis, yAxis);
			if(ikTarget != "-")
				ikRelationships.emplace(preColon, ikTarget);
		}

		map<string, string>::iterator it;
		for(it = ikRelationships.begin(); it != ikRelationships.end(); it++){
			string targetBoneName = it->first;
			string ikTargetBoneName = ikRelationships[targetBoneName];
			skeleton->getBone(targetBoneName)->setIkTarget(skeleton->getBone(ikTargetBoneName));
		}
	}

	KeyframeChannel VbModelReader::readKeyframeChannel(Animatable *animatable, vector<string> &animationData, int &keyframeChannelStartLine){
		string channelData[3];
		getLineData(animationData[keyframeChannelStartLine], channelData, 3);

		string animatableName = channelData[0], typeStr = channelData[1];
		int numKeyframes = atoi(channelData[2].c_str());
		int keyframeStartLine = keyframeChannelStartLine + 1;

		KeyframeChannel keyframeChannel;
		keyframeChannel.animatable = (animatable || !currentSkeleton ? animatable : currentSkeleton->getBone(animatableName));
		keyframeChannel.type = KeyframeChannel::getKeyframeChannelType(typeStr);

		for(int k = 0; k < numKeyframes; k++){
			string keyframeLine = animationData[keyframeStartLine + k];
			string keyframeData[7];
			getLineData(keyframeLine, keyframeData, 7);
		
			Keyframe keyframe;
			keyframe.value = atof(keyframeData[0].c_str());
			keyframe.frame = atof(keyframeData[1].c_str());
			keyframe.interpolation = (KeyframeInterpolation)atoi(keyframeData[2].c_str());
			keyframe.leftHandleValue = atof(keyframeData[3].c_str());
			keyframe.leftHandleFrame = atof(keyframeData[4].c_str());
			keyframe.rightHandleValue = atof(keyframeData[5].c_str());
			keyframe.rightHandleFrame = atof(keyframeData[6].c_str());

			keyframeChannel.keyframes.push_back(keyframe);
		}

		keyframeChannelStartLine += numKeyframes + 1;

		return keyframeChannel;
	}

	void VbModelReader::readAnimations(Animatable *animatable, AnimationController *controller, vector<string> &animationData){
		int numAnimations = atoi(animationData[0].substr(animationData[0].find_first_of(':') + 2).c_str());
		int animStartLine = 1;

		for(int i = 0; i < numAnimations; i++){
			string animLine = animationData[animStartLine];
			string data[2];
			getLineData(animLine.substr(animLine.find_first_of(':') + 2), data, 2);
			currentAnim = data[0];

			Animation *animation = new Animation(currentAnim);
			controller->addAnimation(animation);

			int numKeyframeChannels = atoi(data[1].c_str());
			int keyframeChannelStartLine = animStartLine + 1;
			for(int j = 0; j < numKeyframeChannels; j++)
				animation->addKeyframeChannel(readKeyframeChannel(animatable, animationData, keyframeChannelStartLine));
			
			animStartLine = keyframeChannelStartLine;
		}
	}

	void VbModelReader::readDrivers(vector<string> &meshData, string objectName){
		int numDrivers = atoi(meshData[0].substr(meshData[0].find_first_of(':') + 1).c_str());
		int driverStartLine = 1; 
		const int numData = 3;

		for(int i = 0; i < numDrivers; i++){
			string data[numData];
			getLineData(meshData[driverStartLine], data, numData);
			bool driverIsBone = (data[numData - 1] != "");
			Driver::VariableType type = Driver::getDriverVariableType(driverIsBone ? data[numData - 1] : data[numData - 2]);

			int keyframeChannelStartLine = driverStartLine + 1;
			string drivingNode = data[0];
		   	string drivingBone = (driverIsBone ? data[1] : "");
		   	string drivenAnimatable = objectName;
			string channelData[1];
			getLineData(meshData[keyframeChannelStartLine], channelData, 1);
		   	string drivenSubAnimatable = channelData[0];

			string *driverSetup = new string[4]{drivingNode, drivingBone, drivenAnimatable, drivenSubAnimatable};
			driverSetups.push_back(driverSetup);

			KeyframeChannel keyframeChannel = readKeyframeChannel(nullptr, meshData, keyframeChannelStartLine);

			drivers.push_back(new Driver(keyframeChannel, type));

			driverStartLine += 2 + keyframeChannel.keyframes.size();
		}
	}

	Skeleton* VbModelReader::readSkeleton(vector<string> &skeletonData){
	   	string name = skeletonData[0].substr(getCharId(skeletonData[0], ':') + 2);
		string line = skeletonData[5];
	   	string data[3];
	   	getLineData(line, data, 3);

		int boneStartLine = 6;
		int numBones = atoi(data[1].c_str());
		int animationStartLine = boneStartLine + numBones;
		int driverStartLine = animationStartLine;
		for(int i = animationStartLine; i < skeletonData.size(); i++)
			if(skeletonData[i].find_first_of(':') != -1 && skeletonData[i].substr(0, skeletonData[i].length() - 1) == "drivers: "){
				driverStartLine = i;
				break;
			}

		AnimationController *controller = new AnimationController();
		Skeleton *skeleton = new Skeleton(controller, name);
		currentSkeleton = skeleton;

		vector<string> skeletonSubData = vector<string>(skeletonData.begin() + boneStartLine, skeletonData.begin() + boneStartLine + numBones);
		createBones(skeleton, skeletonSubData);
		skeletonSubData.clear();

		skeletonSubData = vector<string>(skeletonData.begin() + animationStartLine, skeletonData.begin() + driverStartLine);
		readAnimations(nullptr, controller, skeletonSubData);
		skeletonSubData.clear();

		skeletonSubData = vector<string>(skeletonData.begin() + driverStartLine, skeletonData.end());
		readDrivers(skeletonSubData, name);
		skeletonSubData.clear();

		currentSkeleton = nullptr;

		return skeleton;
	}

	void VbModelReader::readVertices(
		   	vector<Vector3> &vertPos,
		   	vector<Vector3> &vertNorm,
		   	vector<float> &vertWeights,
			vector<string> &meshData,
		   	int numBones
			){
		for(string line : meshData){
			int numData = 6 + numBones;
			string data[numData];
			getLineData(line, data, numData);
			Vector3 vPos = Vector3(atof(data[0].c_str()), atof(data[2].c_str()), -atof(data[1].c_str()));
			Vector3 vNorm = Vector3(atof(data[3].c_str()), atof(data[5].c_str()), -atof(data[4].c_str()));
			vertPos.push_back(vPos);
			vertNorm.push_back(vNorm);

			float weightCoefs[numBones];
			for(int i = 0; i < numBones; i++)
				weightCoefs[i] = atof(data[6 + i].c_str());

			int numPosWeights = 0;
			float sumWeightCoefs = 0;
			for(int i = 0; i < numBones; i++)
				if(weightCoefs[i] > 0){
					sumWeightCoefs += weightCoefs[i];
				}

			float root = 1. / sumWeightCoefs;
			for(int i = 0; i < numBones; i++)
				vertWeights.push_back(root * weightCoefs[i]);
		}
	}

	void VbModelReader::readFaces(
		   	vector<Vector3> &vertPos,
		   	vector<Vector3> &vertNorm,
			vector<u32> &vertIds,
		   	vector<float> &vertWeights,
			vector<string> &meshData,
		   	int numBones,
			Mesh::Vertex *vertices,
			u32 *indices
			){
		for(int i = 0; i < meshData.size(); i++){
			int numData = 9;
			string data[numData];
			getLineData(meshData[i], data, numData);
			int index = atoi(data[0].c_str());

			Vector2 uv = Vector2(atof(data[1].c_str()), atof(data[2].c_str()));
			Vector3 tan = Vector3(atof(data[3].c_str()), atof(data[5].c_str()), -atof(data[4].c_str()));
			Vector3 biTan = Vector3(atof(data[6].c_str()), atof(data[8].c_str()), -atof(data[7].c_str())); 

			Mesh::Vertex vert;
			vert.pos = vertPos[index];
			vert.norm = vertNorm[index];
			vert.tan = tan;
			vert.biTan = biTan;
			vert.uv = uv;

			for(int j = 0, boneIndex = 0; j < numBones; j++){
				if(vertWeights[index * numBones + j] > 0){
					vert.boneIndices[boneIndex] = j;
					vert.weights[boneIndex] = vertWeights[index * numBones + j];
					boneIndex++;
				}
			}

			vertIds.push_back(index);
			vertices[i] = vert;
			indices[i] = i;
		}
	}

	void VbModelReader::readVertexGroups(string *groups, vector<string> &meshData, int numBones){
		for(int i = 0; i < numBones; i++){
			groups[i] = meshData[i];
		}
	}

	void VbModelReader::readShapeKeys(Mesh::Vertex *vertices, vector<u32> &vertIds, vector<string> &meshData, Mesh::ShapeKey *shapeKeys, int numShapes, int numVertPos){
		/*
		for(int i = 0; i < vertIds.size(); i++)
			vertices[i].shapeKeyOffsets = new Vector3[numShapes];
			*/

		int shapeKeyStartLine = 0;
		for(int i = 0; i < numShapes; i++){
			string line = meshData[shapeKeyStartLine];
			int colonId = line.find_first_of(':');
			Mesh::ShapeKey shapeKey;
			shapeKey.name = line.substr(0, colonId);
			string data[2];
			getLineData(line.substr(colonId + 2), data, 2);
			shapeKey.minValue = atof(data[0].c_str());
			shapeKey.value = shapeKey.minValue;
			shapeKey.maxValue = atof(data[1].c_str());
			shapeKeys[i] = shapeKey;

			Vector3 *shapeKeyVertPos = new Vector3[numVertPos];
			for(int j = 0; j < numVertPos; j++){
				string line = meshData[shapeKeyStartLine + 1 + j];
				string dataLine[3];
				getLineData(line, dataLine, 3);
				shapeKeyVertPos[j] = Vector3(atof(dataLine[0].c_str()), atof(dataLine[2].c_str()), -atof(dataLine[1].c_str()));
			}
			for(int j = 0; j < vertIds.size(); j++){
				Vector3 offset = shapeKeyVertPos[vertIds[j]] - vertices[j].pos;
				vertices[j].shapeKeyOffsets[i] = (offset);
			}
			delete[] shapeKeyVertPos;


			shapeKeyStartLine = shapeKeyStartLine + numVertPos + 1; 
		}
	}

	Mesh* VbModelReader::readMeshes(vector<string> &meshData){
		string nameLine = meshData[0];
		int nameLineColonId = getCharId(nameLine, ':');
		string preColonNameLine = nameLine.substr(0, nameLineColonId);
		string postColonNameLine = nameLine.substr(nameLineColonId + 2, string::npos);

		string numElementsLine = meshData[6];
		int numElementsColonId = getCharId(numElementsLine, ':');
		string numElementsPreColon = numElementsLine.substr(0, numElementsColonId);
		string numElementsPostColon = numElementsLine.substr(numElementsColonId + 2, string::npos);

	   	string data[4];
		getLineData(numElementsPostColon, data, 4);
		string name = postColonNameLine;

	   	string parentLine = meshData[4];
		int parentLineColonId = getCharId(parentLine, ':');
		string parent = parentLine.substr(parentLineColonId + 2, string::npos);
		relationships.emplace(name, parent);

		const int numVertsPerFace = 3;
		int numVertices = atoi(data[0].c_str());
		int numFaces = atoi(data[1].c_str());
		int numBones = atoi(data[2].c_str());
		int numShapes = atoi(data[3].c_str());
	   	int vertexGroupStartLine = 8;
		int vertexStartLine = vertexGroupStartLine + numBones + 1;
	   	int faceStartLine = vertexStartLine + numVertices + 1;
	   	int shapeKeyStartLine = faceStartLine + numFaces * numVertsPerFace + 1;
		int animationStartLine = shapeKeyStartLine + numShapes * (1 + numVertices);
		int driverStartLine = animationStartLine;
		for(int i = animationStartLine; i < meshData.size(); i++)
			if(meshData[i].find_first_of(':') != -1 && meshData[i].substr(0, meshData[i].length() - 1) == "drivers: "){
				driverStartLine = i;
				break;
			}

		vector<Vector3> vertPos, vertNorm;
		vector<float> vertWeights;
		vector<u32> vertIds;
		Mesh::Vertex *vertices = new Mesh::Vertex[numFaces * numVertsPerFace];
		u32 *indices = new u32[numFaces * numVertsPerFace];
		string *groups = new string[numBones];
		Mesh::ShapeKey *shapeKeys = new Mesh::ShapeKey[numShapes];

		vector<string> meshSubData = vector<string>(meshData.begin() + vertexStartLine, meshData.begin() + vertexStartLine + numVertices);
		readVertices(vertPos, vertNorm, vertWeights, meshSubData, numBones);
		meshSubData.clear();

		meshSubData = vector<string>(meshData.begin() + faceStartLine, meshData.begin() + faceStartLine + numFaces * numVertsPerFace);
		readFaces(vertPos, vertNorm, vertIds, vertWeights, meshSubData, numBones, vertices, indices);
		meshSubData.clear();
		vertPos.clear();
		vertNorm.clear();
		vertWeights.clear();

		meshSubData = vector<string>(meshData.begin() + vertexGroupStartLine, meshData.begin() + vertexGroupStartLine + numBones);
		readVertexGroups(groups, meshSubData, numBones);
		meshSubData.clear();

		meshSubData = vector<string>(meshData.begin() + shapeKeyStartLine, meshData.begin() + (shapeKeyStartLine + numShapes * (1 + numVertices)));
		readShapeKeys(vertices, vertIds, meshSubData, shapeKeys, numShapes, numVertices);
		meshSubData.clear();

		AnimationController *controller = new AnimationController();
		Node *node = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, name, controller);
		nodes.push_back(node);

		meshSubData = vector<string>(meshData.begin() + animationStartLine, meshData.begin() + driverStartLine);
		readAnimations(node, controller, meshSubData);
		meshSubData.clear();

		meshSubData = vector<string>(meshData.begin() + driverStartLine, meshData.end());
		readDrivers(meshSubData, name);
		meshSubData.clear();

		string skeletonLine = meshData[5];
		int skeletonLineColonId = getCharId(skeletonLine, ':');
		string skeletonName = skeletonLine.substr(skeletonLineColonId + 2, string::npos);

		Skeleton *skeleton = nullptr;
		for(Skeleton *sk : skeletons)
			if(sk->getName() == skeletonName)
				skeleton = sk;

		Mesh *mesh = new Mesh(vertices, indices, numFaces, groups, numBones, shapeKeys, numShapes, name);
		meshes.push_back(mesh);
		mesh->setSkeleton(skeleton);
		node->attachMesh(mesh);

		return mesh;
	}

	void VbModelReader::buildMesh(vector<string> &meshData){
		Mesh *mesh = readMeshes(meshData);
		mesh->construct();

	}

	void VbModelReader::readLights(vector<string> &lightData){

	}
}
