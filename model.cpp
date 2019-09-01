#include"model.h"
#include"mesh.h"
#include"material.h"
#include"vector.h"
#include<vector>
#include<iostream>
#include<Importer.hpp>
#include<scene.h>
#include<postprocess.h>

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
		Mesh::Vertex vertices[3*numTris];
		unsigned int indices[3*numTris];
		for(int i=0;i<3*numTris;i++){
			Mesh::Vertex v;
			v.pos.x=mesh->mVertices[i].x;
			v.pos.y=mesh->mVertices[i].y;
			v.pos.z=mesh->mVertices[i].z;

			v.norm.x=mesh->mNormals[i].x;
			v.norm.y=mesh->mNormals[i].y;
			v.norm.z=mesh->mNormals[i].z;

			if(mesh->mTextureCoords[0]){
				v.texCoords.x=mesh->mTextureCoords[0][i].x;
				v.texCoords.y=mesh->mTextureCoords[0][i].y;
			}
			else
				v.texCoords=Vector2::VEC_ZERO;
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

	Model::Model(string path) : Node(){
		Importer importer;
		const aiScene *scene=importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs  |  aiProcess_GenNormals);
		if(!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode){
			cout<<"Failed to load model:"<<importer.GetErrorString()<<endl;
			exit(-1);	
		}
		processNode(scene->mRootNode,scene,this);
	}

	Model::~Model(){}

	void Model::update(){
		Node::update();	
	}

	void Model::setMaterial(Material *mat){
		for(Node *node : children)
			for(Mesh *mesh : node->getMeshes())
				mesh->setMaterial(mat);
		this->material=mat;
	}
}
