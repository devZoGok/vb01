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
	root->start(800, 600, PATH, "Light sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	/*
	 * Places the camera above the platform at an angle.
	*/
	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 1, 1) * 20);
	cam->lookAt(Vector3(0, -1, -1).norm(), Vector3(0, 1, -1).norm());

	string modelPath = MODEL_PATH + "platform.xml";
	AssetManager::getSingleton()->load(modelPath);
	Model *box = new Model(modelPath);

	/*
	 * Creates a material with diffuse, normal, and specular maps attached.
	*/
	Material *mat = new Material(PATH + "texture");
	mat->addBoolUniform("texturingEnabled", true);
	mat->addBoolUniform("lightingEnabled", true);
	mat->addBoolUniform("normalMapEnabled", true);
	mat->addBoolUniform("specularMapEnabled", true);

	string im0[] = {TEX_PATH + "bricks.jpg"};
	string im1[] = {TEX_PATH + "bricksNormal.jpg"};
	string im2[] = {TEX_PATH + "bricksSpecular.jpg"};
	AssetManager::getSingleton()->load(im0[0]);
	AssetManager::getSingleton()->load(im0[1]);
	AssetManager::getSingleton()->load(im0[2]);

	Texture *diffuseTex = new Texture(im0, 1, false);
	mat->addTexUniform("textures[0]", diffuseTex, true);

	Texture *normalTex = new Texture(im1, 1, false);
	mat->addTexUniform("textures[1]", normalTex, true);
	
	Texture *specularTex = new Texture(im2, 1, false);
	mat->addTexUniform("textures[2]", specularTex, true);

	mat->addFloatUniform("specularStrength", 1);
	mat->addFloatUniform("shinyness", 2);
	box->setMaterial(mat);

	rootNode->attachChild(box);

	/*
	 * Creates a spotlight placed at an angle agaist the platform.
	*/
	Light *light = new Light(Light::SPOT);
	light->setColor(Vector3(1, 1, 1));

	Node *lightNode = new Node(Vector3(0, 5, -10), Quaternion(.7, Vector3(1, 0, 0)));
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);

	/*
	 * KeyframeChannel structs determine what object and how it will be animated.
	 * They also use keyframes which in turn are determined by their interpolation types,
	 * values that are returned for animatable parameters, e.g. x coordinate for position 
	 * and frame number.
	*/
	KeyframeChannel kcA;
	kcA.animatable = light->getName();
	kcA.type = KeyframeChannel::SPOTLIGHT_OUTER_ANGLE;
	kcA.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .1, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .7, 60),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .9, 120)
	});

	KeyframeChannel kcB;
	kcB.animatable = light->getName();
	kcB.type = KeyframeChannel::SPOTLIGHT_INNER_ANGLE;
	kcB.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .1, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .7, 60)
	});

	string animName = "spotlightAnim";
	Animation *animation = new Animation(animName);
	animation->addKeyframeChannel(kcA);
	animation->addKeyframeChannel(kcB);

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = AnimationController::getSingleton();
	controller->addAnimation(animation);
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);
	channel->addAnimatable(light);
	channel->setAnimationName(animName);
	channel->setLoop(true);

	while(true)
		root->update();

	return 0;
}
