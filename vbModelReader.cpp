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
			readFile(path, data, key, skeletonBracketIds[key]);
			readSkeleton(data);
			data.clear();
		}
		for(it = meshBracketIds.begin(); it != meshBracketIds.end(); ++it){
			int key = it->first;
			readFile(path, data, key, meshBracketIds[key]);
			readMeshes(data);
			data.clear();
		}
		for(it = lightBracketIds.begin(); it != lightBracketIds.end(); ++it){
			int key = it->first;
			readFile(path, data, key, lightBracketIds[key]);
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
		for(string line : relationships){
			Node *child = nullptr, *parent = nullptr;
			int spaceId = getCharId(line, ' ');
			string childName = line.substr(0, spaceId), parentName = line.substr(spaceId + 1, string::npos);
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

	void VbModelReader::createBones(Skeleton *skeleton, vector<string> &meshData){
		vector<string> ikRelationships;

		for(string line : meshData){
			int colonId = getCharId(line, ':');
			string preColon = line.substr(0, colonId);
			string postColon = line.substr(colonId + 2);

			int numData = 13;
			string data[numData];
			getLineData(postColon, data, numData);

			float length = atof(data[1].c_str());
			Vector3 head = Vector3(atof(data[2].c_str()), atof(data[3].c_str()), atof(data[4].c_str()));
			Vector3 xAxis = Vector3(atof(data[5].c_str()), atof(data[6].c_str()), atof(data[7].c_str()));
			Vector3 yAxis = Vector3(atof(data[8].c_str()), atof(data[9].c_str()), atof(data[10].c_str()));
			string ikTarget = data[11].c_str();
			int ikChainLength = atoi(data[12].c_str());
			Vector3 zAxis = xAxis.cross(yAxis);
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
			bone->lookAt(zAxis, yAxis, parent);
			if(ikTarget != "-")
				ikRelationships.push_back(preColon + " " + ikTarget);
		}

		for(string ikRelationship : ikRelationships){
			int spaceId = getCharId(ikRelationship, ' ');
			string targetBoneName = ikRelationship.substr(0, spaceId);
			string ikTargetBoneName = ikRelationship.substr(spaceId + 1);
			skeleton->getBone(targetBoneName)->setIkTarget(skeleton->getBone(ikTargetBoneName));
		}
	}

	void VbModelReader::readAnimations(Skeleton *skeleton, vector<string> &meshData){
		AnimationController *controller = skeleton->getAnimationController();
		for(string line : meshData){
			int colonId = line.find_first_of(':');
			if(colonId != -1){
				string data[2];
				string postCol = line.substr(colonId + 1);
				getLineData(postCol, data, 2);
				string animName = data[0];
				int numBones = atoi(data[1].c_str());
				controller->addAnimation(new Animation(animName));
			}
		}
	}

	void VbModelReader::readKeyframesGroups(Skeleton *skeleton, vector<string> &meshData){
		AnimationController *controller = skeleton->getAnimationController();
		for(string line : meshData){
			string data[17];
			getLineData(line, data, 3);

			Animation *anim = controller->getAnimation(data[0]);
			Bone *animBone = skeleton->getBone(data[1]);
			int numChannelTypes = atoi(data[2].c_str());

			getLineData(line, data, numChannelTypes * 2, 3);
			KeyframeGroup keyframeGroup;
			for(int i = 0; i < numChannelTypes * 2; i++){
				string typeString = data[i * 2];
				int numChannels = atoi(data[i * 2 + 1].c_str());
				KeyframeChannelType type = anim->getKeyframeChannelType(typeString);

				for(int j = 0; j < numChannels; j++){
					KeyframeChannel keyframeChannel;
					keyframeChannel.type = type;
					keyframeGroup.keyframeChannels.push_back(keyframeChannel);
				}
			}
			anim->addKeyframeGroup(keyframeGroup);
		}
	}

	void VbModelReader::readKeyframes(Skeleton *skeleton, vector<string> &meshData){
		AnimationController *controller = skeleton->getAnimationController();
		for(string line : meshData){
			int numData = 6;
			string data[numData];
			getLineData(line, data, numData);
			Animation *anim = controller->getAnimation(data[0]);
			Bone *animBone = skeleton->getBone(data[1]); 
			KeyframeChannelType type = anim->getKeyframeChannelType(data[2]);
			KeyframeChannel channel = anim->getKeyframeChannel(animBone, type);

			Keyframe keyframe;
			keyframe.value = atof(data[3].c_str());
			keyframe.frame = atoi(data[4].c_str());
			keyframe.interpolation = (KeyframeInterpolation)atoi(data[5].c_str());
			channel.keyframes.push_back(keyframe);
		}
	}

	void VbModelReader::readSkeleton(vector<string> &skeletonData){
		//vector<string> skeletonMetaData(skeletonData.begin() + 1, skeletonData.begin() + 7);
	   	string name = skeletonData[0].substr(getCharId(skeletonData[0], ':') + 2, string::npos);
		string line = skeletonData[5];
	   	string data[3];
	   	getLineData(line, data, 3);

		int numBones = atoi(data[1].c_str());
		int numKeyframeGroups = -2;
		int numAnimations = atoi(data[2].c_str());

		int boneStartLine = 7;
		int animationStartLine = boneStartLine + numBones + 1;
		int keyframeGroupStartLine = animationStartLine + numKeyframeGroups + 1;
		int keyframeStartLine = animationStartLine + numAnimations + 1;

		Skeleton *skeleton = new Skeleton(name);
		skeletons.push_back(skeleton);

		vector<string> *skeletonSubData = new vector<string>(skeletonData.begin() + boneStartLine, skeletonData.begin() + boneStartLine + numBones);
		createBones(skeleton, *skeletonSubData);
		skeletonSubData->clear();
		delete skeletonSubData;
		
		skeletonSubData = new vector<string>(skeletonData.begin() + animationStartLine, skeletonData.begin() + animationStartLine + numAnimations);
		readAnimations(skeleton, *skeletonSubData);
		skeletonSubData->clear();
		delete skeletonSubData;

		skeletonSubData = new vector<string>(skeletonData.begin() + keyframeGroupStartLine, skeletonData.begin() + keyframeGroupStartLine + numKeyframeGroups);
		readKeyframesGroups(skeleton, *skeletonSubData);
		skeletonSubData->clear();
		delete skeletonSubData;

		skeletonSubData = new vector<string>(skeletonData.begin() + keyframeGroupStartLine, skeletonData.end());
		readKeyframes(skeleton, *skeletonSubData);
		skeletonSubData->clear();
		delete skeletonSubData;
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

			for(int j = 0; j < numBones; j++)
				vertWeights.push_back(atof(data[6 + j].c_str()));
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
		groups = new string[numBones];
		for(int i = 0; i < numBones; i++){
			groups[i] = meshData[i];
		}
	}

	void VbModelReader::readShapeKeys(int shapeKeyStartLine, int numShapes){
		for(int i = 0; i < numShapes; i++){
		}
	}

	void VbModelReader::readMeshes(vector<string> &meshData){
		//readFile(path, meshData, startLine + 1, startLine + 8);

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
		relationships.push_back(postColonNameLine + " " + parent);

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

		vector<string> vertexData(meshData.begin() + vertexStartLine, meshData.begin() + vertexStartLine + numVertices);
		readVertices(vertPos, vertNorm, vertWeights, vertexData, numBones);
		vertexData.clear();

		vector<string> faceData(meshData.begin() + faceStartLine, meshData.begin() + faceStartLine + numFaces * numVertsPerFace);
		readFaces(vertPos, vertNorm, vertWeights, faceData, numBones, vertices, indices);
		faceData.clear();

		vector<string> vertexGroupData(meshData.begin() + vertexGroupStartLine, meshData.begin() + vertexGroupStartLine + numBones);
		readVertexGroups(groups, vertexGroupData, numBones);
		vertexGroupData.clear();

		vector<string> shapeKeyData(meshData.begin() + shapeKeyStartLine, meshData.begin() + shapeKeyStartLine + numShapes);
		readShapeKeys(shapeKeyStartLine, numShapes);
		shapeKeyData.clear();

		string skeletonLine = meshData[5];
		int skeletonLineColonId = getCharId(skeletonLine, ':');
		string skeletonName = skeletonLine.substr(skeletonLineColonId + 2, string::npos);

		Skeleton *skeleton = nullptr;
		for(Skeleton *sk : skeletons)
			if(sk->getName() == skeletonName)
				skeleton = sk;

		Mesh *mesh = new Mesh(vertices, indices, numFaces, groups, numBones, nullptr, numShapes, name);
		mesh->setSkeleton(skeleton);

		Node *node = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, name);
		node->attachMesh(mesh);
		nodes.push_back(node);
	}

	void VbModelReader::readLights(vector<string> &lightData){

	}
}
