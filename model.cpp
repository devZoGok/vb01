#include"model.h"
#include"mesh.h"
#include"material.h"
#include"vector.h"
#include"util.h"
#include"bone.h"
#include"root.h"
#include"skeleton.h"
#include"animation.h"

#include<vector>
#include<iostream>
#include<Importer.hpp>
#include<scene.h>
#include<postprocess.h>
#include<fstream>

using namespace std;
using namespace Assimp;

namespace vb01{
	void Model::processNode(aiNode *node, const aiScene *scene, Node *currentNode){
		for(int i=0;i<node->mNumMeshes;i++){
			aiMesh *mesh=scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh,scene,currentNode);
		}	
		for(int i=0;i<node->mNumChildren;i++){
			Node *childNode=new Node();
			currentNode->attachChild(childNode);
			processNode(node->mChildren[i],scene,childNode);
		}
	}

	void Model::processMesh(aiMesh *mesh, const aiScene *scene, Node *currentNode){
		const int numTris=mesh->mNumFaces;
		Mesh::Vertex *vertices=new Mesh::Vertex[3*numTris];
		u32 *indices=new u32[3*numTris];
		for(int i=0;i<3*numTris;i++){
			Mesh::Vertex v;
			v.pos.x=mesh->mVertices[i].x;
			v.pos.y=mesh->mVertices[i].y;
			v.pos.z=mesh->mVertices[i].z;

			v.norm.x=mesh->mNormals[i].x;
			v.norm.y=mesh->mNormals[i].y;
			v.norm.z=mesh->mNormals[i].z;

			v.tan.x=mesh->mTangents[i].x;
			v.tan.y=mesh->mTangents[i].y;
			v.tan.z=mesh->mTangents[i].z;

			v.biTan.x=mesh->mBitangents[i].x;
			v.biTan.y=mesh->mBitangents[i].y;
			v.biTan.z=mesh->mBitangents[i].z;

			if(mesh->mTextureCoords[0]){
				v.uv.x=mesh->mTextureCoords[0][i].x;
				v.uv.y=mesh->mTextureCoords[0][i].y;
			}
			else
				v.uv=Vector2::VEC_ZERO;
			vertices[i]=v;
		}				
		for(int i=0;i<numTris;i++){
			int numFaceIndices=mesh->mFaces[i].mNumIndices;
			for(int j=0;j<numFaceIndices;j++){
				indices[numFaceIndices*i+j]=mesh->mFaces[i].mIndices[j];		
			}
		}
		//if(mesh->mMaterialIndex>=0){
		//	aiMaterial *material=scene->mMaterial[mesh->mMaterialIndex];
		//}
		currentNode->attachMesh(new Mesh(vertices,indices,numTris));
	}

	Model::Model(string path,bool b) : Node(){
		Node *rootNode=Root::getSingleton()->getRootNode();
		if(b){
			Importer importer;
			const aiScene *scene=importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
			if(!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode){
				cout<<"Failed to load model:"<<importer.GetErrorString()<<endl;
				exit(-1);	
			}
			processNode(scene->mRootNode,scene,this);
		}
		else{
			enum Stage{ARMATURE,MESH};

			Stage stage;
			ifstream in(path);
			string l,name="";
			Vector3 localPos=Vector3::VEC_ZERO,localScale=Vector3::VEC_IJK;
			Quaternion localRot=Quaternion::QUAT_W;
			vector<string> childrenChain;
			vector<Skeleton*> skeletons;
			vector<Mesh*> meshes;
			vector<Light*> lights;
			vector<Node*> nodes;

			Skeleton *skeleton=nullptr;
			string animName="";
			Bone *animBone=nullptr;

			vector<Vector3> vertPos,vertNorm;
			const int numVertsPerFace=3;
			int numVertPos=0,numFaces=0,vertId=0,faceId=0,groupId=-1,groupVertId=0,numGroups=0,numShapeKeys=0;
			Mesh::Vertex *vertices;
			u32 *indices; 
			Mesh::VertexGroup *groups=nullptr;
			Mesh::ShapeKey *shapeKeys=nullptr;

			while(getline(in,l)){
				int colonId=-1;
				for(int i=0;i<l.length();i++)
					if(l[i]==':'){
						colonId=i;
						break;
					}

				string preColon=l.substr(0,colonId);
				string postColon=l.length()==colonId+1?"":l.substr(colonId+2,string::npos);

				if(preColon=="ARMATURE"){
					stage=ARMATURE;
					skeleton=new Skeleton(postColon);
					name=postColon;
					continue;
				}
				else if(preColon=="MESH"){
					stage=MESH;
					name=postColon;
					continue;
				}
				else if(preColon=="pos"){
					continue;
				}
				else if(preColon=="rot"){
					continue;
				}
				else if(preColon=="scale"){
					continue;
				}
				else if(preColon=="parent"){
					childrenChain.push_back(name+" "+postColon);
					continue;
				}

				if(stage==ARMATURE){
					enum ArmatureStage{BONES,ANIMATIONS};

					ArmatureStage armatureStage;
					int numBones=0;

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

							float length=atof(data[0].c_str());
							Vector3 pos=Vector3(atof(data[2].c_str()),atof(data[4].c_str()),-atof(data[3].c_str()));
							Vector3 xAxis=Vector3(atof(data[5].c_str()),atof(data[7].c_str()),-atof(data[6].c_str())),yAxis=Vector3(atof(data[8].c_str()),atof(data[10].c_str()),-atof(data[9].c_str())),zAxis=xAxis.cross(yAxis);

							string parName=string(data[0].c_str());
							Node *parent=skeleton->getBone(parName);
							if(!parent)
								parent=this;

							Bone *bone=new Bone(preColon);
							skeleton->addBone(bone,parent);
							bone->setPosition(parent->globalToLocalPosition(pos));
							bone->lookAt(yAxis,zAxis,parent);

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
				else{
					enum MeshStage{VERTICES,FACES,GROUPS,SHAPE_KEYS};

					MeshStage meshStage;

					if(preColon=="vertices"){
						string data[2];
						getLineData(postColon,data,2);
						numVertPos=atoi(data[0].c_str());
						numFaces=atoi(data[1].c_str());
						meshStage=VERTICES;
						continue;
					}
					else if(preColon=="faces"){
						meshStage=FACES;
						vertices=new Mesh::Vertex[numFaces*numVertsPerFace];
						indices=new u32[numFaces*numVertsPerFace];
						continue;
					}
					else if(preColon=="groups"){
						numGroups=atoi(postColon.c_str());
						groups=new Mesh::VertexGroup[numGroups];
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
							int numData=6;
							string data[numData];
							getLineData(l,data,numData);
							Vector3 vp=Vector3(atof(data[0].c_str()),atof(data[2].c_str()),-atof(data[1].c_str()));
							Vector3 vn=Vector3(atof(data[3].c_str()),atof(data[5].c_str()),-atof(data[4].c_str()));
							vertPos.push_back(vp);
							vertNorm.push_back(vn);
							break;
						}
						{
						case FACES:
							int numData=9;
							string data[numData];
							getLineData(l,data,numData);
							u32 index=atoi(data[0].c_str());
							Vector2 uv=Vector2(atof(data[1].c_str()),atof(data[2].c_str()));
							Vector3 tan=Vector3(atof(data[3].c_str()),atof(data[5].c_str()),-atof(data[4].c_str()));
							Vector3 biTan=Vector3(atof(data[6].c_str()),atof(data[8].c_str()),-atof(data[7].c_str()));
							/*
							*/

							Mesh::Vertex vert;
							vert.pos=vertPos[index];
							vert.norm=vertNorm[index];
							vert.uv=uv;
							vert.tan=tan;
							vert.biTan=biTan;
							vertices[vertId]=vert;

							indices[vertId]=vertId;
							vertId++;
							break;
						}
						{
						case GROUPS:
							if(l==""){
								meshes.push_back(new Mesh(vertices,indices,numFaces,groups,numGroups,shapeKeys,numShapeKeys,name));
								break;
							}
							else if(colonId!=-1){
								groupId++,groupVertId=0;
								int numVerts=atoi(postColon.c_str());
								groups[groupId].numVertices=numVerts;
								groups[groupId].name=preColon;
								if(numVerts>0){
									groups[groupId].vertices=new u32[numVerts];
									groups[groupId].weights=new float[numVerts];
								}
							}
							else{
								int numData=2,vertId;
								float weight;
								string data[numData];
								getLineData(l,data,numData);
								vertId=atoi(data[0].c_str());
								weight=atof(data[1].c_str());
								groups[groupId].vertices[groupVertId]=vertId;
								groups[groupId].weights[groupVertId]=weight;
								groupVertId++;
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
			}
			for(Mesh *mesh : meshes){
				Node *meshNode=new Node(localPos,localRot,localScale);
				meshNode->attachMesh(mesh);
				nodes.push_back(meshNode);
				for(string line : childrenChain){
					int spaceId=-1;
					for(int i=0;i<line.length();i++)
						if(line[i]==' ')
							spaceId=i;
					string childName=line.substr(0,spaceId),parentName=line.substr(spaceId+1,string::npos);
					if(mesh->getName()==childName){
						for(Skeleton *sk : skeletons){
							if(sk->getName()==parentName)
								mesh->setSkeleton(sk);
						}
					}
				}
			}
			for(Light *light : lights){
				Node *lightNode=new Node(localPos,localRot,localScale);
				lightNode->addLight(light);
				nodes.push_back(lightNode);
			}
			for(string line : childrenChain){
				int spaceId=-1;
				for(int i=0;i<line.length();i++)
					if(line[i]==' ')
						spaceId=i;
				string childName=line.substr(0,spaceId),parentName=line.substr(spaceId+1,string::npos);
				Node *childNode=nullptr,*parentNode=this;
				for(int i=0;i<nodes.size();i++){
					Mesh *m0=nodes[i]->getMesh(0);
					Skeleton *sk0=m0->getSkeleton();
					if(m0->getName()==childName||(sk0&&sk0->getName()==childName)){
						childNode=nodes[i];
						for(int j=0;j<nodes.size();j++){
							Mesh *m1=nodes[j]->getMesh(0);
							Skeleton *sk1=m1->getSkeleton();
							if(m1->getName()==parentName||(sk1&&sk1->getName()==parentName))
								parentNode=nodes[j];
						}
					}
				}
				if(childNode!=parentNode)
					parentNode->attachChild(childNode);
			}
		}
	}

	Model::~Model(){
		delete children[0]->getMesh(0)->getMaterial();

		vector<Node*> descendants;
		getDescendants(this,descendants);
		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setMaterial(nullptr);

		while(!children.empty()){
			Node *c=children[children.size()-1];
			dettachChild(c);
			delete c;
		}
	}

	void Model::update(){
		Node::update();	
	}

	void Model::getLineData(string &line,string data[],int numData,int offset){
		int offsetComma=0,c1=0;
		for(int i=0;i<line.length()&&offsetComma<offset;i++)
			if(line.c_str()[i]==' '){
				c1=i+1;
				offsetComma++;
			}
		int c2=c1;
		for(int i=0;i<numData;i++){
			for(int j=c1;j<line.length();j++)	
				if(line.c_str()[j]==' '){
					c2=j;
					break;
				}
			data[i]=line.substr(c1,c2-c1);
			c2++;
			c1=c2;
		}
	}

	void Model::setMaterial(Material *mat){
		vector<Node*> descendants;
		getDescendants(this,descendants);
		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setMaterial(mat);
	}

	void Model::setCastShadow(bool castShadow){
		vector<Node*> descendants;
		getDescendants(this,descendants);
		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setCastShadow(castShadow);
		this->castShadow=castShadow;
	}

	void Model::setReflect(bool reflect){
		vector<Node*> descendants;
		getDescendants(this,descendants);
		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setReflect(reflect);
		this->reflect=reflect;
	}

	void Model::setWireframe(bool wirefame){
		vector<Node*> descendants;
		getDescendants(this,descendants);
		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setWireframe(wirefame);
		this->wireframe=wireframe;
	}
}
