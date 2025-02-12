#include <iostream>

#include "root.h"
#include "assetManager.h"
#include "quad.h"
#include "box.h"
#include "node.h"
#include "material.h"
#include "rayCaster.h"

using namespace std;
using namespace vb01;

int main(){
	const string PATH = "../", TEX_PATH = PATH + "samples/textures/", MODEL_PATH = PATH + "samples/models/";
	const int numTextures = 6;
	string skyboxTextures[numTextures] = {
		TEX_PATH + "top.jpg",
		TEX_PATH + "bottom.jpg",
		TEX_PATH + "left.jpg",
		TEX_PATH + "right.jpg",
		TEX_PATH + "front.jpg",
		TEX_PATH + "back.jpg"
	};

	for(int i = 0; i < numTextures; i++)
		AssetManager::getSingleton()->load(skyboxTextures[i]);

	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Plane ray cast sample");
	root->createSkybox(skyboxTextures);

	Camera *cam = root->getCamera();
	cam->setFov(70);
	cam->setFarPlane(500);
	cam->setPosition(70 * Vector3::VEC_IJK);
	cam->lookAt(-Vector3::VEC_IJK, Vector3(-1, 1, -1).norm());

	Material *mat = new Material(PATH + "texture");
	mat->addBoolUniform("lightingEnabled", false);
	mat->addBoolUniform("texturingEnabled", false);
	mat->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 1));

	Vector3 size = Vector3(100, 100, 0);
	int numVertDivs = 11, numHorDivs = 11;
	Quad *quad = new Quad(size, true, numVertDivs, numHorDivs);
	quad->setMaterial(mat);

	Node *plane = new Node();
	plane->attachMesh(quad);

	Node *rootNode = root->getRootNode();
	rootNode->attachChild(plane);

	float stepX = (numHorDivs > 0 ? size.x / numHorDivs : size.x);
	float stepZ = (numVertDivs > 0 ? size.y / numVertDivs : size.y);
	Vector3 startPos = -.5 * (Vector3(size.x, 0, size.y) - Vector3(stepX, 0, stepZ));

	for(int i = 0; i < numVertDivs; i++){
		for(int j = 0; j < numHorDivs; j++){
			Vector3 rayPos = startPos + Vector3(stepX * j, 0, stepZ * i);
			vector<RayCaster::CollisionResult> results = RayCaster::cast(startPos + Vector3(0, 100, 0), -Vector3::VEC_J, plane, 0, 30);
			bool hit = !results.empty();

			if(!hit)
				cout << "x: " << rayPos.x << ", z: " << rayPos.z << endl;

			Material *mat = new Material(PATH + "texture");
			mat->addBoolUniform("lightingEnabled", false);
			mat->addBoolUniform("texturingEnabled", false);
			mat->addVec4Uniform("diffuseColor", (hit ? Vector4(0, 1, 0, 1) : Vector4(1, 0, 0, 1)));

			Box *box = new Box(Vector3::VEC_IJK * .7);
			box->setMaterial(mat);

			Node *node = new Node(rayPos);
			node->attachMesh(box);

			rootNode->attachChild(node);
		}
	}

	while(true){
		root->update();
	}

	return 0;
}
