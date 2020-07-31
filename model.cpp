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

			const int numVertsPerFace=3;
			int numFaces=0,vertId=0,faceId=0,groupId=0,groupVertId=0,numGroups=0,numShapeKeys=0,numBones=0;
			Mesh::Vertex *vertices;
			u32 *indices; 
			vector<Vector3> vertPos,vertNorm;
			vector<float> vertWeights;
			string *groups=nullptr,*shapeKeys=nullptr;

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
