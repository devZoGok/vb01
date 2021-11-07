#include "root.h"
#include "model.h"
#include "material.h"
#include "mesh.h"
#include "skeleton.h"
#include "animationController.h"
#include "animationChannel.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace vb01;

int main(){
	const string PATH = "../", TEX_PATH = PATH + "samples/textures/", MODEL_PATH = PATH + "samples/models/";

	string skyboxTextures[] = {
		TEX_PATH + "top.jpg",
		TEX_PATH + "bottom.jpg",
		TEX_PATH + "left.jpg",
		TEX_PATH + "right.jpg",
		TEX_PATH + "front.jpg",
		TEX_PATH + "back.jpg"
	};

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
	cam->setPosition(Vector3(0, 0, 1) * 20);
	cam->lookAt(Vector3(0, 0, -1).norm(), Vector3(0, 1, 0).norm());

	Model *model = new Model(MODEL_PATH + "kek.vb");
	Material *mat = new Material(PATH + "texture");
	mat->addVariable("lightingEnabled", false);
	mat->addVariable("texturingEnabled", true);
	string images[] = {TEX_PATH + "defaultTexture.jpg"};
	Texture *texture = new Texture(images, 1);
	mat->addVariable("textures[0]", texture);
	model->setMaterial(mat);
	rootNode->attachChild(model);

	int numChildren = model->getNumChildren();
	Node *node = model->getChild(1);
	Mesh *mesh = node->getMesh(0);
	Skeleton *skeleton = mesh->getSkeleton();

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = skeleton->getAnimationController();
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);
	channel->addAnimatable(skeleton->getBone("handIK.R"));
	channel->addAnimatable(skeleton->getBone("elbowIK.R"));
	channel->setAnimationName("righArmAnim");
	channel->setLoop(true);

	skeleton->getBone("handIK.L")->setPosePos(Vector3(1.5, -1.5, 0));

	while(true){
		root->update();
	}

	return 0;
}
