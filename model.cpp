#include"model.h"
#include"mesh.h"
#include"material.h"
#include"vector.h"
#include"util.h"
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

	Model::Model(string path,bool b) : Node(){
		if(b){
		Importer importer;
		const aiScene *scene=importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs  |  aiProcess_GenNormals);
		if(!scene||scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode){
			cout<<"Failed to load model:"<<importer.GetErrorString()<<endl;
			exit(-1);	
		}
		processNode(scene->mRootNode,scene,this);
		}
		else{
		int numTris,numVerts,numGroups,v=0,u=0,vg=0,b=0;
		Mesh::Vertex *vertices;
		Mesh::VertexGroup *groups=new Mesh::VertexGroup[5];
		unsigned int *indices;
		Vector3 *pos,*norm;

		ifstream in(path);
		string l;
		bool verts=false,uv=false,vertexGroups=false,bones=false;

		while(getline(in,l)){
			if(l.substr(0,8)=="numFaces"){
				numTris=atoi(l.substr(9,string::npos).c_str());		
				vertices=new Mesh::Vertex[numTris*3];
				indices=new unsigned int[numTris*3];
			}
			else if(l.substr(0,11)=="numVertices"){
				numVerts=atoi(l.substr(12,string::npos).c_str());		
				pos=new Vector3[numVerts];
				norm=new Vector3[numVerts];
			}
			else if(l.substr(0,9)=="numGroups"){
				numGroups=atoi(l.substr(10,string::npos).c_str());		
				groups=new Mesh::VertexGroup[numGroups];
			}
			if(l=="vertices:"){
				verts=true;
				continue;
			}
			else if(l=="uv:"){
				verts=false;	
				uv=true;
				continue;
			}
			else if(l=="groups:"){
				uv=false;
				vertexGroups=true;
				continue;
			}
			else if(l=="bones:"){
				vertexGroups=false;
				bones=true;
				continue;
			}

			if(verts||uv||vertexGroups||bones){
				int numCoords=1;
				if(verts)numCoords=6;
				else if(uv)numCoords=3;
				else if(vg)numCoords=3;
				else if(groups)numCoords=1;

				int nextSpace=0;
				int *spaceIds=new int[numCoords-1];
				for(int i=0;i<l.length();i++)
					if(l.c_str()[i]==' '){
						spaceIds[nextSpace]=i;
						nextSpace++;
					}
				float *coords=new float[numCoords];
				for(int i=0;i<numCoords;i++){
					int firstChar,lastChar;
					if(i==0)
						firstChar=0,lastChar=spaceIds[i];
					else if(i==numCoords-1)
						firstChar=spaceIds[i-1],lastChar=string::npos;
					else
						firstChar=spaceIds[i-1],lastChar=spaceIds[i]-spaceIds[i-1];
					coords[i]=atof(l.substr(firstChar,lastChar).c_str());
				}
				if(verts){
					pos[v]=Vector3(coords[0],coords[1],coords[2]);
					norm[v]=Vector3(coords[3],coords[4],coords[5]);
					//Vector2 uv=Vector2(coords[6],coords[7]);
					//vertices[v].pos=pos;
					//vertices[v].norm=norm;
					//vertices[v].texCoords=uv;
					v++;
				}
				else if(uv){
					int id=(int)coords[0];
					Vector2 uv=Vector2(coords[1],coords[2]);
					vertices[u].pos=pos[id];
					vertices[u].norm=norm[id];
					vertices[u].texCoords=uv;
					indices[u]=u;
					u++;
				}
				else if(vertexGroups){
					if(l.c_str()[l.length()-1]==':'){
						numCoords=1;
						groups[vg].vertices=new Mesh::Vertex*;
						groups[vg].weights=new float;
						groups[vg].name=l.c_str()[l.length()-1];
						vg++;
					}
					else{
						numCoords=2;
						int *ids=new int,numVerts=0;
						for(int i=0;i<3*numTris;i++){
							if(vertices[i].pos==pos[(int)coords[0]]){
								ids[numVerts]=i;
								numVerts++;
							}
						}
						for(int i=0;i<3*numTris;i++)
							for(int j=0;j<numVerts;j++){
								groups[vg].vertices[groups[vg].numVertices]=&(vertices[ids[j]]);
								groups[vg].weights[ids[j]]=coords[1];
							}
						groups[vg].numVertices++;
						delete[] ids;
					}
				}
				else if(bones){
					b++;
				}
				delete[] coords;
			}
		}
		delete[] pos;
		delete[] norm;
		attachChild(new Node());
		children[0]->attachMesh(new Mesh(vertices,indices,numTris));
		}
	}

	Model::~Model(){}

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
		for(Node *node : children)
			for(Mesh *mesh : node->getMeshes())
				mesh->setCastShadow(castShadow);
		this->castShadow=castShadow;
	}
}
