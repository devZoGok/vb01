#include "root.h"
#include "node.h"
#include "box.h"
#include "material.h"
#include "keyframeChannel.h"
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
	root->start(800, 600, PATH, "Driver sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	/*
	 * Places the camera at an angle to the cubes.
	*/
	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(1, 1, 1) * 2);
	cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());

	/*
	 * Populate the scene with cubes
	*/
	Vector3 size = Vector3(1, 1, 1) * .25;
	Box *driverBox = new Box(size);
	Node *driver = new Node();
	string shaderPath = PATH + "texture";
	Material *driverMat = new Material(shaderPath);
	driverMat->addBoolUniform("texturingEnabled", false);
	driverMat->addVec4Uniform("diffuseColor", Vector4(0, 1, 0, 1));
	driverBox->setMaterial(driverMat);
	driver->attachMesh(driverBox);
	rootNode->attachChild(driver);

	Box *leftBox = new Box(size);
	Node *leftNode = new Node();
	Material *leftMat = new Material(shaderPath);
	leftMat->addBoolUniform("texturingEnabled", false);
	leftMat->addVec4Uniform("diffuseColor", Vector4(1, 0, 0, 1));
	leftBox->setMaterial(leftMat);
	leftNode->attachMesh(leftBox);
	rootNode->attachChild(leftNode);
	leftNode->setPosition(Vector3(-1, 0, 0));

	Box *rightBox = new Box(size);
	Node *rightNode = new Node();
	Material *rightMat = new Material(shaderPath);
	rightMat->addBoolUniform("texturingEnabled", false);
	rightMat->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 1));
	rightBox->setMaterial(rightMat);
	rightNode->attachMesh(rightBox);
	rootNode->attachChild(rightNode);
	rightNode->setPosition(Vector3(1, 0, 0));

	/*
	 * KeyframeChannel structs determine what object and how it will be animated.
	 * They also use keyframes which in turn are determined by their interpolation types,
	 * values that are returned for animatable parameters, e.g. x coordinate for position 
	 * and frame number.
	*/
	KeyframeChannel kcL;
	kcL.animatable = leftMat->getUniform("diffuseColor")->getName();
	kcL.type = KeyframeChannel::UNIFORM_1;
	kcL.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 0),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 0, 1)
	});

	KeyframeChannel kcR;
	kcR.animatable = rightNode->getName();
	kcR.type = KeyframeChannel::SCALE_Z;
	kcR.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 0),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 0, 1)
	});

	KeyframeChannel kcD;
	kcD.animatable = driver->getName();
	kcD.type = KeyframeChannel::POS_Y;
	kcD.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 0, 0),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 29),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 0, 40)
	});

	/*
	 * A Driver class is given a transform component, e.g, y coordinate of the position
	 * and uses it to play the KeyframeChannel.
	*/
	driver->addDriver(new Driver(leftMat->getUniform("diffuseColor"), kcL, Driver::POS_Y));
	driver->addDriver(new Driver(rightNode, kcR, Driver::POS_Y));
	
	string animName = "anim";
	Animation *anim = new Animation(animName);
	anim->addKeyframeChannel(kcD);

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = AnimationController::getSingleton();
	controller->addAnimation(anim);
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);
	channel->addAnimatable(driver);
	channel->setAnimationName(animName);
	channel->setLoop(true);

	while(true){
		root->update();
	}

	return 0;
}
