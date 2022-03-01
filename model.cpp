#include "model.h"
#include "mesh.h"
#include "material.h"
#include "vector.h"
#include "util.h"
#include "bone.h"
#include "root.h"
#include "skeleton.h"
#include "animation.h"
#include "vbModelReader.h"
#include "xmlModelReader.h"
#include "assimpModelReader.h"

#include <vector>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <fstream>

using namespace std;
using namespace Assimp;

namespace vb01{
	Model::Model(string path) : Node(){
		int dotId = getCharId(path, '.', true);
		string extension = path.substr(dotId + 1, string::npos);
		ModelReader *modelReader = nullptr;

		if(extension == "xml")
			modelReader = new XmlModelReader(this, path);
		else if(extension == "vb")
			modelReader = new VbModelReader(this, path);
		else
			modelReader = new AssimpModelReader(this, path);
	}

	Model::~Model(){
		delete children[0]->getMesh(0)->getMaterial();

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
		vector<Node*> descendants;
		getDescendants(descendants);

		for(Node *node : descendants)
			for(Mesh *mesh : node->getMeshes())
				mesh->setMaterial(mat);
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
