#include "root.h"
#include "box.h"
#include "material.h"
#include "node.h"
#include "light.h"
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

	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Light sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 1, 1) * 30);
	cam->lookAt(Vector3(0, -1, -1).norm(), Vector3(0, 1, -1).norm());

	Box *box = new Box(Vector3(20, 1, 20));
	Node *boxNode = new Node();

	Material *mat = new Material();
	mat->setTexturingEnabled(true);
	mat->setLightingEnabled(true);
	string images[] = {TEX_PATH + "bricks.jpg"};
	Texture *texture = new Texture(images, 1);
	mat->addDiffuseMap(texture);
	box->setMaterial(mat);

	boxNode->attachMesh(box);
	rootNode->attachChild(boxNode);

	Light *light = new Light(Light::SPOT);
	light->setColor(Vector3(1, 1, 1));
	Node *lightNode = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "", new AnimationController());
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);
	lightNode->setOrientation(Quaternion(1.57, Vector3(1, 0, 0)));
	lightNode->setPosition(Vector3(0, 5, 0));

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
