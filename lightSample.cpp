#include "root.h"
#include "box.h"
#include "material.h"
#include "node.h"
#include "light.h"
#include "model.h"
#include "animation.h"
#include "animationController.h"
#include "animationChannel.h"

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
	root->start(800, 600, PATH, "Light sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	/*
	 * Places the camera above the platform at an angle.
	*/
	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 1, 1) * 20);
	cam->lookAt(Vector3(0, -1, -1).norm(), Vector3(0, 1, -1).norm());

	Model *box = new Model(MODEL_PATH + "platform.vb");

	/*
	 * Creates a material with diffuse, normal, and specular maps attached.
	*/
	Material *mat = new Material(PATH + "texture");
	mat->addBoolUniform("texturingEnabled", true);
	mat->addBoolUniform("lightingEnabled", true);
	mat->addBoolUniform("normalMapEnabled", true);
	mat->addBoolUniform("specularMapEnabled", true);

	string im0[] = {TEX_PATH + "bricks.jpg"};
	Texture *diffuseTex = new Texture(im0, 1);
	mat->addTexUniform("textures[0]", diffuseTex, true);

	string im1[] = {TEX_PATH + "bricksNormal.jpg"};
	Texture *normalTex = new Texture(im1, 1, Texture::NORMAL);
	mat->addTexUniform("textures[1]", normalTex, true);
	
	string im2[] = {TEX_PATH + "bricksSpecular.jpg"};
	Texture *specularTex = new Texture(im2, 1, Texture::SPECULAR);
	mat->addTexUniform("textures[2]", specularTex, true);

	//Setting specular parameteres
	mat->addFloatUniform("specularStrength", 1);
	mat->addFloatUniform("shinyness", 2);
	box->setMaterial(mat);

	rootNode->attachChild(box);

	/*
	 * Creates a spotlight placed at an angle agaist the platform.
	*/
	Light *light = new Light(Light::SPOT);
	light->setColor(Vector3(1, 1, 1));

	Node *lightNode = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "", new AnimationController());
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);
	lightNode->setOrientation(Quaternion(.7, Vector3(1, 0, 0)));
	lightNode->setPosition(Vector3(0, 5, -10));

	/*
	 * KeyframeChannel structs determine what object and how it will be animated.
	 * They also use keyframes which in turn are determined by their interpolation types,
	 * values that are returned for animatable parameters, e.g. x coordinate for position 
	 * and frame number.
	*/
	KeyframeChannel kcA;
	kcA.animatable = light;
	kcA.type = KeyframeChannel::SPOTLIGHT_OUTER_ANGLE;
	kcA.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .1, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .7, 60),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .9, 120)
	});

	KeyframeChannel kcB;
	kcA.animatable = light;
	kcA.type = KeyframeChannel::SPOTLIGHT_INNER_ANGLE;
	kcA.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .1, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .7, 60)
	});

	Animation *animation = new Animation("spotlightAnim");
	animation->addKeyframeChannel(kcA);
	animation->addKeyframeChannel(kcB);

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = lightNode->getAnimationController();
	controller->addAnimation(animation);
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);
	channel->addAnimatable(light);
	channel->setLoop(true);
	channel->setAnimationName("spotlightAnim");

	while(true)
		root->update();

	return 0;
}
