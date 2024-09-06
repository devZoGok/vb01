#include "model.h"
#include "mesh.h"
#include "material.h"
#include "vector.h"
#include "util.h"
#include "bone.h"
#include "root.h"
#include "skeleton.h"
#include "animation.h"
#include "modelAsset.h"
#include "assetManager.h"

using namespace std;

namespace vb01{
	Model::Model(string path) : Node(){
			ModelAsset *asset = ((ModelAsset*)AssetManager::getSingleton()->getAsset(path));
			Node *clonedRoot = asset->rootNode->clone();
			vector<Node*> descendants = vector<Node*>{clonedRoot};
			clonedRoot->getDescendants(descendants);

			for(Node *desc : descendants)
					for(Mesh *mesh : desc->getMeshes())
							mesh->construct();

			for(Node *child : clonedRoot->getChildren())
					attachChild(child);
	}

	Model::~Model(){
		vector<Node*> descendants;
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setMaterial(nullptr);

		while(!children.empty()){
			Node *c = children[children.size() - 1];
			dettachChild(c);
			delete c;
		}
	}

	void Model::update(){
		Node::update();	
	}

	void Model::setMaterial(Material *mat){
		vector<Node*> descendants = vector<Node*>{this};
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setMaterial(mat);

		this->material = mat;
	}

	void Model::setCastShadow(bool castShadow){
		vector<Node*> descendants;
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setCastShadow(castShadow);

		this->castShadow = castShadow;
	}

	void Model::setReflect(bool reflect){
		vector<Node*> descendants;
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setReflect(reflect);

		this->reflect = reflect;
	}

	void Model::setReflective(bool reflective){
		vector<Node*> descendants;
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setReflect(reflect);

		this->reflective = reflective;
	}

	void Model::setWireframe(bool wireframe){
		vector<Node*> descendants;
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setWireframe(wireframe);

		this->wireframe = wireframe;
	}
}
