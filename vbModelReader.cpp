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

		const string meshType = "MESH", skeletonType = "ARMATURE", lightType = "LIGHT";
		vector<int> meshBracketIds, skeletonBracketIds, lightBracketIds;	
		int curBracketId = -1, lineIndex = 0;
		string line, type;
		ifstream file(path);

		while(getline(file, line)){
			if(line[0] == '{' || line[0] == '}'){
				curBracketId = lineIndex;
				if(line[0] == '{'){
					getline(file, line);
					lineIndex++;

					int colonId = getCharId(line, ':');
					type = line.substr(0, colonId);
				}
				if(type == meshType)
					meshBracketIds.push_back(curBracketId);
				else if(type == skeletonType)
					skeletonBracketIds.push_back(curBracketId);
				else if(type == lightType)
					lightBracketIds.push_back(curBracketId);
			}
			lineIndex++;
		}

		file.close();

		for(int i = 0; i < skeletonBracketIds.size() / 2; i++)
			readSkeletons(skeletonBracketIds[i * 2], skeletonBracketIds[i * 2 + 1]);
		for(int i = 0; i < meshBracketIds.size() / 2; i++)
			readMeshes(meshBracketIds[i * 2], meshBracketIds[i * 2 + 1]);
		for(int i = 0; i < lightBracketIds.size() / 2; i++)
			readLights(lightBracketIds[i * 2], lightBracketIds[i * 2 + 1]);

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

	VbModelReader::~VbModelReader(){
	}

	void VbModelReader::readBones(Skeleton *skeleton, int boneStartLine, int numBones){
		vector<string> meshData;
		readFile(path, meshData, boneStartLine, boneStartLine + numBones);
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
				swap(xAxis.y, xAxis.z);
				xAxis.z = -xAxis.z;
				swap(yAxis.y, yAxis.z);
				yAxis.z = -yAxis.z;
				swap(zAxis.y, zAxis.z);
				zAxis.z = -zAxis.z;
				swap(pos.y, pos.z);
				pos.z = -pos.z;
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

	void VbModelReader::readAnimations(Skeleton *skeleton, int animationStartLine, int endLine){
		AnimationController *controller = skeleton->getAnimationController();
		string animName = "";
		Bone *animBone = nullptr;
		Animation::KeyframeGroup::KeyframeChannel::Type type;
		vector<string> meshData;
		readFile(path, meshData, animationStartLine, endLine + 1);

		for(string l : meshData){
			int colonId = getCharId(l, ':');
			string preColon = l.substr(0, colonId);
			if(preColon == "animationName"){
				string postColon = l.substr(colonId + 2, string::npos);
				animName = postColon;
				controller->addAnimation(new Animation(animName));
				continue;
			}
			Bone *b = skeleton->getBone(preColon);
			if(b){
				animBone = b;
				Animation *anim = controller->getAnimation(animName);
				Animation::KeyframeGroup *group = anim->getKeyframeGroup(animBone);

				if(!group){
					Animation::KeyframeGroup kg;
					kg.bone = animBone;
					anim->addKeyframeGroup(kg);
				}
			}
			else{
				KeyframeGroup *keyframeGroup = controller->getAnimation(animName)->getKeyframeGroup(animBone);
				KeyframeInterpolation interp;
				KeyframeChannel channel;

				if(l == "}")
					break;
				else if(preColon == "pos_x" || preColon == "pos_y" || preColon == "pos_z" || preColon == "rot_w" || preColon == "rot_x" || preColon == "rot_y" || preColon == "rot_z"){
					if(preColon == "pos_x")
						type = KeyframeChannelType::POS_X;
					else if(preColon == "pos_y")
						type = KeyframeChannelType::POS_Y;
					else if(preColon == "pos_z")
						type = KeyframeChannelType::POS_Z;
					else if(preColon == "rot_w")
						type = KeyframeChannelType::ROT_W;
					else if(preColon == "rot_x")
						type = KeyframeChannelType::ROT_X;
					else if(preColon == "rot_y")
						type = KeyframeChannelType::ROT_Y;
					else if(preColon == "rot_z")
						type = KeyframeChannelType::ROT_Z;

					channel.type = type;
					keyframeGroup->keyframeChannels.push_back(channel);
				}
				else{
					int numData = 3;
					string data[numData];

					getLineData(l, data, numData);

					float value = atof(data[0].c_str()), frame = atoi(data[1].c_str());
					interp = (KeyframeInterpolation)atoi(data[2].c_str());

					Animation::KeyframeGroup::KeyframeChannel::Keyframe keyframe;
					//keyframe.type = type;
					keyframe.interpolation = interp;
					keyframe.value = value;
					keyframe.frame = frame;
					for(KeyframeChannel &channel : keyframeGroup->keyframeChannels)
						if(channel.type == type)
							channel.keyframes.push_back(keyframe);
					//skeleton->getAnimationController()->getAnimation(animName)->getKeyframeGroup(animBone)->keyframes.push_back(keyframe);
				}
			}
		}
	}

	void VbModelReader::readSkeletons(int startLine, int endLine){
		vector<string> meshData;
		readFile(path, meshData, startLine + 1, startLine + 7);
	   	string name = meshData[0].substr(getCharId(meshData[0], ':') + 2, string::npos);
		string line = meshData[5];
	   	string data[3];
	   	getLineData(line, data, 3);

		int numBones = atoi(data[1].c_str());
		int numAnimations = atoi(data[2].c_str());
		int boneStartLine = startLine + 7;
		int animationStartLine = boneStartLine + numBones + 1;

		Skeleton *skeleton = new Skeleton(name);
		skeletons.push_back(skeleton);

		readBones(skeleton, boneStartLine, numBones);
		
		readAnimations(skeleton, animationStartLine, endLine);

	}

	void VbModelReader::readVertices(
		   	vector<Vector3> &vertPos,
		   	vector<Vector3> &vertNorm,
		   	vector<float> &vertWeights,
		   	int vertexStartLine,
		   	int numVertices,
		   	int numBones
			){
		vector<string> meshData;
		readFile(path, meshData, vertexStartLine, vertexStartLine + numVertices);
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
		   	int faceStartLine,
		   	int numFaces,
		   	int numVertsPerFace,
		   	int numBones,
			Mesh::Vertex *vertices,
			u32 *indices
			){
		int i = 0;
		vector<string> meshData;
		readFile(path, meshData, faceStartLine, faceStartLine + numFaces * numVertsPerFace);
		for(string line : meshData){
			int numData = 9;
			string data[numData];
			getLineData(line,data,numData);
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

			i++;
		}
	}

	void VbModelReader::readMeshes(int startLine, int endLine){
		vector<string> meshData;
		readFile(path, meshData, startLine + 1, startLine + 8);

		string nameLine = meshData[0], parentLine = meshData[4], skeletonLine = meshData[5], numElementsLine = meshData[6];

		int skeletonLineColonId = getCharId(skeletonLine, ':');
		string skeletonName = skeletonLine.substr(skeletonLineColonId + 2, string::npos);

		int parentLineColonId = getCharId(parentLine, ':');
		string parent = parentLine.substr(parentLineColonId + 2, string::npos);

		int nameLineColonId = getCharId(nameLine, ':');
		string preColonNameLine = nameLine.substr(0, nameLineColonId);
		string postColonNameLine = nameLine.substr(nameLineColonId + 2, string::npos);

		int numElementsColonId = getCharId(numElementsLine, ':');
		string numElementsPreColon = numElementsLine.substr(0, numElementsColonId);
		string numElementsPostColon = numElementsLine.substr(numElementsColonId + 2, string::npos);

	   	string data[4];
		getLineData(numElementsPostColon, data, 4);
		string name = postColonNameLine;

		relationships.push_back(postColonNameLine + " " + parent);

		const int numVertsPerFace = 3;
		int numVertices = atoi(data[0].c_str());
		int numFaces = atoi(data[1].c_str());
		int numBones = atoi(data[2].c_str());
		int numShapes = atoi(data[3].c_str());
	   	int vertexGroupStartLine = startLine + 9;
		int vertexStartLine = vertexGroupStartLine + numBones + 1;
	   	int faceStartLine = vertexStartLine + numVertices + 1;
	   	int shapeKeyStartLine = faceStartLine + numFaces + 1;

		vector<Vector3> vertPos, vertNorm;
		vector<float> vertWeights;
		Mesh::Vertex *vertices = new Mesh::Vertex[numFaces * numVertsPerFace];
		u32 *indices = new u32[numFaces * numVertsPerFace];
		string *groups = new string[numBones];

		meshData.clear();
		readFile(path, meshData, vertexGroupStartLine, vertexGroupStartLine + numBones);
		groups = new string[numBones];
		for(int i = 0; i < numBones; i++){
			groups[i] = meshData[i];
		}

		readVertices(vertPos, vertNorm, vertWeights, vertexStartLine, numVertices, numBones);

		readFaces(vertPos, vertNorm, vertWeights, faceStartLine, numFaces, numVertsPerFace, numBones, vertices, indices);

		meshData.clear();
		readFile(path, meshData, shapeKeyStartLine, shapeKeyStartLine + numShapes);
		for(int i = 0; i < numShapes; i++){
		}

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

	void VbModelReader::readLights(int startLine, int endLine){

	}
}
