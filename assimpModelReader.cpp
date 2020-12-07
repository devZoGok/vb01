#include"assimpModelReader.h"
#include"mesh.h"
#include<Importer.hpp>
#include<scene.h>
#include<postprocess.h>

using namespace std;
using namespace Assimp;

namespace vb01{
	AssimpModelReader::AssimpModelReader(Model *model, string path) : ModelReader(model, path){
		Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			cout<<"Failed to load model:"<<importer.GetErrorString()<<endl;
			exit(-1);	
		}
		processNode(scene->mRootNode, scene, model);
	}

	void AssimpModelReader::processNode(aiNode *node, const aiScene *scene, Node *currentNode){
		for(int i = 0; i < node->mNumMeshes; i++){
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene, currentNode);
		}	
		for(int i = 0;i < node->mNumChildren; i++){
			Node *childNode = new Node();
			currentNode->attachChild(childNode);
			processNode(node->mChildren[i], scene, childNode);
		}
	}

	void AssimpModelReader::processMesh(aiMesh *mesh, const aiScene *scene, Node *currentNode){
		const int numTris = mesh->mNumFaces;
		Mesh::Vertex *vertices = new Mesh::Vertex[3 * numTris];
		u32 *indices = new u32[3 * numTris];
		for(int i = 0; i < 3 * numTris; i++){
			Mesh::Vertex v;
			v.pos.x = mesh->mVertices[i].x;
			v.pos.y = mesh->mVertices[i].y;
			v.pos.z = mesh->mVertices[i].z;

			v.norm.x = mesh->mNormals[i].x;
			v.norm.y = mesh->mNormals[i].y;
			v.norm.z = mesh->mNormals[i].z;

			v.tan.x = mesh->mTangents[i].x;
			v.tan.y = mesh->mTangents[i].y;
			v.tan.z = mesh->mTangents[i].z;

			v.biTan.x = mesh->mBitangents[i].x;
			v.biTan.y = mesh->mBitangents[i].y;
			v.biTan.z = mesh->mBitangents[i].z;

			if(mesh->mTextureCoords[0]){
				v.uv.x = mesh->mTextureCoords[0][i].x;
				v.uv.y = mesh->mTextureCoords[0][i].y;
			}
			else
				v.uv = Vector2::VEC_ZERO;
			vertices[i] = v;
		}				
		for(int i = 0; i < numTris; i++){
			int numFaceIndices = mesh->mFaces[i].mNumIndices;
			for(int j = 0; j < numFaceIndices; j++){
				indices[numFaceIndices * i + j] = mesh->mFaces[i].mIndices[j];		
			}
		}
		//if(mesh->mMaterialIndex>=0){
		//	aiMaterial *material=scene->mMaterial[mesh->mMaterialIndex];
		//}
		currentNode->attachMesh(new Mesh(vertices, indices, numTris));
	}
}
