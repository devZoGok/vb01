#include "root.h"
#include "model.h"
#include "material.h"
#include "mesh.h"
#include "skeleton.h"
#include "animationController.h"
#include "animationChannel.h"
#include "box.h"
#include "assetManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

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
	root->start(800, 600, PATH, "Morph animation sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	/*
	 * Places the camera to the side of the model.
	*/
	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(-1, 0.25, .2) * 20);
	cam->lookAt(Vector3(1, 0, 0).norm(), Vector3(0, 1, 0).norm());

	string modelPath = MODEL_PATH + "kek.xml";
	AssetManager::getSingleton()->load(modelPath);
	Model *model = new Model(modelPath);

	Material *mat = new Material(PATH + "texture");
	mat->addBoolUniform("lightingEnabled", false);
	mat->addBoolUniform("texturingEnabled", true);

	string images[] = {TEX_PATH + "defaultTexture.jpg"};
	AssetManager::getSingleton()->load(images[0]);
	Texture *texture = new Texture(images, 1, false);
	mat->addTexUniform("textures[0]", texture, true);

	model->setMaterial(mat);
	rootNode->attachChild(model);
	Skeleton *skeleton = model->getChild(0)->getSkeleton(0);

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = AnimationController::getSingleton();
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);
	channel->addAnimatable(skeleton->getBone("bone"));
	channel->setAnimationName("morph");
	channel->setLoop(true);

	/*
	 * Creates a box attached to the bone controlling the shape keys.
	*/
	Box *box = new Box(Vector3(1, 1, 1) * .25);
	Material *boxMat = new Material(PATH + "texture");
	boxMat->addBoolUniform("texturingEnabled", false);
	boxMat->addVec4Uniform("diffuseColor", Vector4(1, 0, 0, 1));
	box->setMaterial(boxMat);
	skeleton->getBone("bone")->attachMesh(box);

	while(true){
		root->update();
	}

	return 0;
}
