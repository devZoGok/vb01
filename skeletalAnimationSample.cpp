#include "root.h"
#include "model.h"
#include "material.h"
#include "mesh.h"
#include "skeleton.h"
#include "animationController.h"
#include "animationChannel.h"
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
	root->start(800, 600, PATH, "Skeletal animation sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	/*
	 * Places the camera in front of the model.
	*/
	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 0.25, 1) * 20);
	cam->lookAt(Vector3(0, 0, -1).norm(), Vector3(0, 1, 0).norm());

	AssetManager::getSingleton()->load(MODEL_PATH + "kek.xml");

	Model *model = new Model(MODEL_PATH + "kek.xml");
	Material *mat = new Material(PATH + "texture");

	string images[] = {TEX_PATH + "defaultTexture.jpg"};
	AssetManager::getSingleton()->load(images[0]);

	Texture *texture = new Texture(images, 1, false);
	mat->addTexUniform("textures[0]", texture, true);
	mat->addBoolUniform("lightingEnabled", false);
	mat->addBoolUniform("texturingEnabled", true);

	model->setMaterial(mat);
	rootNode->attachChild(model);

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = AnimationController::getSingleton();
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);

	/*
	 * Here we see that it is possible to transform skeleton
	 * bones either manually or via an animation.
	*/
	Skeleton *skeleton = model->getChild(0)->getSkeleton(0);
	skeleton->getBone("handIK.L")->setPosePos(Vector3(1.5, -1.5, 0));

	channel->addAnimatable(skeleton->getBone("handIK.R"));
	channel->addAnimatable(skeleton->getBone("elbowIK.R"));
	channel->setAnimationName("righArmAnim");
	channel->setLoop(true);

	while(true){
		root->update();
	}

	return 0;
}
