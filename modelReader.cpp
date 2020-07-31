#include"modelReader.h"
#include<vector>
#include<fstream>

using namespace std;

namespace vb01{
	ModelReader::ModelReader(string path){
		this->path = path;

		const string meshType = "MESH", skeletonType = "ARMATURE", lightType = "LIGHT";
		vector<int> meshBracketIds, skeletonBracketIds, lightBracketIds;	
		int curBracketId = -1;
		string line;
		ifstream file(path);

		for(int i = 0; getline(file, line); i++){
			if(line[0] == '{' || line[0] == '}')
				curBracketId = i;
			else{
				int colonId = getCharId(':');

				string type = line.substr(0, colonId - 1);
				if(type == meshType)
					meshBracketIds.push_back(curBracketId);
				else if(type == skeletonType)
					skeletonBracketIds.push_back(curBracketId);
				else if(type == lightType)
					lightBracketIds.push_back(curBracketId);
			}
		}

		file.close();

		for(int i = 0; i < meshBracketIds.size() / 2; i++)
			readMeshes(meshBracketIds[i * 2], meshBracketIds[i * 2 + 1]);
		for(int i = 0; i < skeletonBracketIds.size() / 2; i++)
			readSkeletons(skeletonBracketIds[i * 2], skeletonBracketIds[i * 2 + 1]);
		for(int i = 0; i < lightBracketIds.size() / 2; i++)
			readLights(lightBracketIds[i * 2], lightBracketIds[i * 2 + 1]);
	}

	ModelReader::~ModelReader(){
	}

	void ModelReader::readSkeletons(int startLine, int endLine){
		/*
				if(stage==ARMATURE){
					enum ArmatureStage{BONES,ANIMATIONS};

					ArmatureStage armatureStage;

					if(preColon=="bones"){
						numBones=atoi(postColon.c_str());
						armatureStage=BONES;
						continue;
					}
					else if(preColon=="animations"){
						armatureStage=ANIMATIONS;
						continue;
					}

					switch(armatureStage){
						case BONES:
						{
							int numData=11;
							string data[numData];
							getLineData(postColon,data,numData);

							float length = atof(data[1].c_str());
							Vector3 head=Vector3(atof(data[2].c_str()),atof(data[3].c_str()),atof(data[4].c_str()));
							Vector3 xAxis=Vector3(atof(data[5].c_str()),atof(data[6].c_str()),atof(data[7].c_str()));
							Vector3 yAxis=Vector3(atof(data[8].c_str()),atof(data[9].c_str()),atof(data[10].c_str()));
							Vector3 zAxis=xAxis.cross(yAxis);
							Vector3 pos = head;

							string parName=string(data[0].c_str());
							Node *parent=skeleton->getBone(parName);
							if(!parent){
								parent=this;
								swap(xAxis.y,xAxis.z);
								xAxis.z=-xAxis.z;
								swap(yAxis.y,yAxis.z);
								yAxis.z=-yAxis.z;
								swap(zAxis.y,zAxis.z);
								zAxis.z=-zAxis.z;
							}
							else
								pos = pos + Vector3(0,((Bone*)parent)->getLength(),0);

							Bone *bone=new Bone(preColon,length,pos);
							skeleton->addBone(bone,(Bone*)parent);
							bone->lookAt(zAxis,yAxis,parent);

							break;
						}
						case ANIMATIONS:
						{
							if(preColon=="animationName"){
								animName=postColon;
								skeleton->addAnimation(new Animation(animName));
								continue;
							}
							Bone *b=skeleton->getBone(preColon);
							if(b){
								animBone=b;
								Animation *anim=skeleton->getAnimation(animName);
								Animation::KeyframeGroup *group=anim->getKeyframeGroup(animBone);
								if(!group){
									Animation::KeyframeGroup kg;
									kg.bone=animBone;
									anim->addKeyframeGroup(kg);
								}
								continue;
							}
							else{
								Animation::KeyframeGroup::Keyframe::Type type;
								Animation::KeyframeGroup::Keyframe::Interpolation interp;
								if(preColon=="pos_x")
									type=Animation::KeyframeGroup::Keyframe::Type::POS_X;
								else if(preColon=="pos_y")
									type=Animation::KeyframeGroup::Keyframe::Type::POS_Y;
								else if(preColon=="pos_z")
									type=Animation::KeyframeGroup::Keyframe::Type::POS_Z;
								else if(preColon=="rot_w")
									type=Animation::KeyframeGroup::Keyframe::Type::ROT_W;
								else if(preColon=="rot_x")
									type=Animation::KeyframeGroup::Keyframe::Type::ROT_X;
								else if(preColon=="rot_y")
									type=Animation::KeyframeGroup::Keyframe::Type::ROT_Y;
								else if(preColon=="rot_z")
									type=Animation::KeyframeGroup::Keyframe::Type::ROT_Z;
								else if(l==""){
									skeletons.push_back(skeleton);
									break;
								}
								else{
									int numData=3;
									string data[numData];

									getLineData(l,data,numData);

									float value=atof(data[0].c_str()),frame=atoi(data[1].c_str());
									interp=(Animation::KeyframeGroup::Keyframe::Interpolation)atoi(data[2].c_str());

									Animation::KeyframeGroup::Keyframe keyframe;
									keyframe.type=type;
									keyframe.interpolation=interp;
									keyframe.value=value;
									keyframe.frame=frame;
									skeleton->getAnimation(animName)->getKeyframeGroup(animBone)->keyframes.push_back(keyframe);

								}
							}
							break;
						}
					}
				}
		 */
	}

	void ModelReader::readMeshes(int startLine, int endLine){
		vector<string> meshData;
		readFile(path, meshData, startLine - 4, endLine - 1);
		for(String line : meshData){
			int colonId = getCharId(':');
			string preColonSubstring = line.substr(0, colonId - 1);
			string postColonSubstring = line.substr(colonId + 1, string::npos);
		}

		numFaces=atoi(data[1].c_str());
		numGroups=atoi(data[2].c_str());
		vertices=new Mesh::Vertex[numFaces*numVertsPerFace];
		indices=new u32[numFaces*numVertsPerFace];

			if(preColon=="vertices"){
				const int numData=4;
				string data[numData];
				getLineData(postColon,data,numData);
				meshStage=VERTICES;
				continue;
			}
			else if(preColon=="faces"){
				meshStage=FACES;
				continue;
			}
			else if(preColon=="groups"){
				groups=new string[numGroups];
				meshStage=GROUPS;
				continue;
			}
			else if(preColon=="shapeKeys"){
				meshStage=SHAPE_KEYS;
				continue;
			}
			switch(meshStage){
				{
				case VERTICES:
					int numData = 6 + numBones;
					string data[numData];
					getLineData(l,data,numData);
					Vector3 vPos=Vector3(atof(data[0].c_str()),atof(data[2].c_str()),-atof(data[1].c_str()));
					Vector3 vNorm=Vector3(atof(data[3].c_str()),atof(data[5].c_str()),-atof(data[4].c_str()));
					vertPos.push_back(vPos);
					vertNorm.push_back(vNorm);
					for(int j=0;j<numBones;j++)
						vertWeights.push_back(atof(data[6 + j].c_str()));
					break;
				}
				{
				case FACES:
					int numData=9;
					string data[numData];
					getLineData(l,data,numData);
					int index=atoi(data[0].c_str());
					Vector2 uv=Vector2(atof(data[1].c_str()),atof(data[2].c_str()));
					Vector3 tan=Vector3(atof(data[3].c_str()),atof(data[5].c_str()),-atof(data[4].c_str()));
					Vector3 biTan=Vector3(atof(data[6].c_str()),atof(data[8].c_str()),-atof(data[7].c_str())); 

					Mesh::Vertex vert;
					vert.pos=vertPos[index];
					vert.norm=vertNorm[index];
					vert.tan=tan;
					vert.biTan=biTan;
					vert.uv=uv;
					for(int j = 0, boneIndex = 0; j < numBones; j++){
						if(vertWeights[index * numBones + j] > 0){
							vert.boneIndices[boneIndex] = j;
							vert.weights[boneIndex] = vertWeights[index * numBones + j];
							boneIndex++;
						}
					}
					indices[vertId]=vertId;
					vertices[vertId]=vert;

					vertId++;
					break;
				}
				{
				case GROUPS:
					if(l==""){
						meshes.push_back(new Mesh(vertices,indices,numFaces,groups,numGroups,shapeKeys,numShapeKeys,name));
						break;
					}
					else{
						groups[groupId]=l;
						groupId++;
					}
					break;
				}
				{
				case SHAPE_KEYS:
					if(l==""){
						meshes.push_back(new Mesh(vertices,indices,numFaces,groups,numGroups,shapeKeys,numShapeKeys,name));
						break;
					}
					break;
				}
			}
	}

	void ModelReader::readLights(int startLine, int endLine){

	}
}
