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
				/*
				*/
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

	void VbModelReader::readAnimations(Skeleton *skeleton, vector<string> &skeletonData, int numAnimations){
		AnimationController *controller = skeleton->getAnimationController();
		int animStartLine = 0;

		for(int i = 0; i < numAnimations; i++){
			string animLine = skeletonData[animStartLine];
			currentAnim = animLine.substr(animLine.find_first_of(':') + 2);
			string groupsLine = skeletonData[animStartLine + 1];

			Animation *animation = new Animation(currentAnim);
			controller->addAnimation(animation);

			int numKeyframeGroups = atoi(groupsLine.substr(groupsLine.find_first_of(':') + 2).c_str());
			int keyframeGroupStartLine = animStartLine + 2;
			for(int j = 0; j < numKeyframeGroups; j++){
				string dataLine[2];
				getLineData(skeletonData[keyframeGroupStartLine], dataLine, 2);

				KeyframeGroup keyframeGroup;
				keyframeGroup.bone = skeleton->getBone(dataLine[0]);
				int numKeyframeChannels = atoi(dataLine[1].c_str());
				string channelData[2 * numKeyframeChannels];
				getLineData(skeletonData[keyframeGroupStartLine], channelData, 2 * numKeyframeChannels, 2);

				vector<KeyframeChannel> keyframeChannels;
				int numTypeKeyframes[numKeyframeChannels];
				int numKeyframes = 0;
				for(int k = 0; k < numKeyframeChannels; k++){
					KeyframeChannel keyframeChannel;

					keyframeChannel.type = Animation::getKeyframeChannelType(channelData[2 * k]);
					numTypeKeyframes[k] = atoi(channelData[2 * k + 1].c_str());
					numKeyframes += numTypeKeyframes[k];

					keyframeChannels.push_back(keyframeChannel);
				}

				int keyframeStartLine = keyframeGroupStartLine + 1;
				int currentChannel = 0, numCurrentTypeKeyframes = 0;
				for(int k = 0; k < numKeyframes; k++){
					string keyframeLine = skeletonData[keyframeStartLine + k];
					string keyframeData[9];
					getLineData(keyframeLine, keyframeData, 9);

					Keyframe keyframe;
					keyframe.value = atof(keyframeData[0].c_str());
					keyframe.frame = atoi(keyframeData[1].c_str());
					keyframe.interpolation = (KeyframeInterpolation)atoi(keyframeData[2].c_str());
					keyframeChannels[currentChannel].keyframes.push_back(keyframe);

					numCurrentTypeKeyframes++;
					if(numCurrentTypeKeyframes == numTypeKeyframes[currentChannel]){
						currentChannel++;
						numCurrentTypeKeyframes = 0;
					}
				}

				keyframeGroup.keyframeChannels = keyframeChannels;
				animation->addKeyframeGroup(keyframeGroup);
				keyframeGroupStartLine += numKeyframes + 1;
			}

			animStartLine = keyframeGroupStartLine;
		}
	}

	Skeleton* VbModelReader::readSkeleton(vector<string> &skeletonData){
	   	string name = skeletonData[0].substr(getCharId(skeletonData[0], ':') + 2);
		string line = skeletonData[5];
	   	string data[3];
	   	getLineData(line, data, 3);

		int boneStartLine = 6;
		int numBones = atoi(data[1].c_str());

		line = skeletonData[boneStartLine + numBones].substr(getCharId(skeletonData[boneStartLine + numBones], ':') + 2);
		int numAnimations = atoi(line.c_str());
		int animStartLine = boneStartLine + numBones + 1;

		Skeleton *skeleton = new Skeleton(name);

		vector<string> skeletonSubData = vector<string>(skeletonData.begin() + boneStartLine, skeletonData.begin() + boneStartLine + numBones);
		createBones(skeleton, skeletonSubData);
		skeletonSubData.clear();

		skeletonSubData = vector<string>(skeletonData.begin() + animStartLine, skeletonData.end());
		readAnimations(skeleton, skeletonSubData, numAnimations);
		skeletonSubData.clear();

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

			vertices[i] = vert;
			indices[i] = i;
		}
	}

	void VbModelReader::readVertexGroups(string *groups, vector<string> &meshData, int numBones){
		for(int i = 0; i < numBones; i++){
			groups[i] = meshData[i];
		}
	}

	void VbModelReader::readShapeKeys(int shapeKeyStartLine, int numShapes){
		for(int i = 0; i < numShapes; i++){
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
	   	int shapeKeyStartLine = faceStartLine + numFaces + 1;

		vector<Vector3> vertPos, vertNorm;
		vector<float> vertWeights;
		Mesh::Vertex *vertices = new Mesh::Vertex[numFaces * numVertsPerFace];
		u32 *indices = new u32[numFaces * numVertsPerFace];
		string *groups = new string[numBones];

		vector<string> meshSubData = vector<string>(meshData.begin() + vertexStartLine, meshData.begin() + vertexStartLine + numVertices);
		readVertices(vertPos, vertNorm, vertWeights, meshSubData, numBones);
		meshSubData.clear();

		meshSubData = vector<string>(meshData.begin() + faceStartLine, meshData.begin() + faceStartLine + numFaces * numVertsPerFace);
		readFaces(vertPos, vertNorm, vertWeights, meshSubData, numBones, vertices, indices);
		meshSubData.clear();

		meshSubData = vector<string>(meshData.begin() + vertexGroupStartLine, meshData.begin() + vertexGroupStartLine + numBones);
		readVertexGroups(groups, meshSubData, numBones);
		meshSubData.clear();

		meshSubData = vector<string>(meshData.begin() + shapeKeyStartLine, meshData.begin() + shapeKeyStartLine + numShapes);
		readShapeKeys(shapeKeyStartLine, numShapes);
		meshSubData.clear();

		string skeletonLine = meshData[5];
		int skeletonLineColonId = getCharId(skeletonLine, ':');
		string skeletonName = skeletonLine.substr(skeletonLineColonId + 2, string::npos);

		Skeleton *skeleton = nullptr;
		for(Skeleton *sk : skeletons)
			if(sk->getName() == skeletonName)
				skeleton = sk;

		Mesh *mesh = new Mesh(vertices, indices, numFaces, groups, numBones, nullptr, numShapes, name);
		mesh->setSkeleton(skeleton);
		return mesh;
	}

	void VbModelReader::buildMesh(vector<string> &meshData){
		Mesh *mesh = readMeshes(meshData);
		mesh->construct();

		Node *node = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, mesh->getName());
		node->attachMesh(mesh);
		nodes.push_back(node);
	}

	void VbModelReader::readLights(vector<string> &lightData){

	}
}
