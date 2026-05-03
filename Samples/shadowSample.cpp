#include "root.h"
#include "box.h"
#include "material.h"
#include "node.h"
#include "light.h"
#include "model.h"
#include "animation.h"
#include "animationController.h"
#include "animationChannel.h"
#include "assetManager.h"

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

	//Loads the assets to be stored and retrievable at a later time
	for(int i = 0; i < numTextures; i++)
		AssetManager::getSingleton()->load(skyboxTextures[i]);

	/*
	 * Gets Root object to start the sample, 
	 * also passes the base path for asset retrieval. 
	*/
	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Shadow sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	/*
	 * Places the camera above the platform at an angle.
	*/
	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 1, 1) * 20);
	cam->lookAt(Vector3(0, -1, -1).norm(), Vector3(0, 1, -1).norm());

	/*
	 * Creates a plaform for the shadow to be cast upon
	*/
	Box *box = new Box(Vector3(40, .2, 40));
	Node *boxNode = new Node();
	Material *boxMat = new Material(PATH + "texture");
	boxMat->addBoolUniform("texturingEnabled", true);
	boxMat->addBoolUniform("lightingEnabled", true);
	string im0[] = {TEX_PATH + "bricks.jpg"};
	AssetManager::getSingleton()->load(im0[0]);
	Texture *boxTex = new Texture(im0, 1, false);
	boxMat->addTexUniform("textures[0]", boxTex, true);
	box->setMaterial(boxMat);
	boxNode->attachMesh(box);
	rootNode->attachChild(boxNode);

	/*
	 * Creates a model that casts a shadow 
	*/
	string modelPath = MODEL_PATH + "jet00.xml";
	AssetManager::getSingleton()->load(modelPath);
	Model *jet = new Model(modelPath);
	Material *jetMat = new Material(PATH + "texture");
	jetMat->addBoolUniform("texturingEnabled", true);
	jetMat->addBoolUniform("lightingEnabled", true);
	string im1[] = {TEX_PATH + "defaultTexture.jpg"};
	AssetManager::getSingleton()->load(im1[0]);
	Texture *jetTex = new Texture(im1, 1, false);
	jetMat->addTexUniform("textures[0]", jetTex, true);
	jet->setMaterial(jetMat);
	rootNode->attachChild(jet);
	jet->setCastShadow(true);
	jet->setPosition(Vector3(0, 2, 0));

	/*
	 * Creates a light according to which the shadow will be cast
	 * Lights are transformed by their node: 
	 * light position = node position, light orientation = node orientation
	*/
	Light *light = new Light(Light::POINT);
	light->setColor(Vector3(1, 1, 1));
	light->setAttenuationValues(.001, .001, 1);

	Node *lightNode = new Node();
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);
	lightNode->setPosition(Vector3(0, 5, 0));

	float angle = 0;

	while(true){
		jet->setOrientation(Quaternion(angle, Vector3(1, 1, 1).norm()));
		angle += .1;

		root->update();
	}

	return 0;
}
