#include "root.h"
#include "node.h"
#include "particleEmitter.h"
#include "material.h"
#include "texture.h"
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

	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Particle emitter sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 0, 1) * 1);
	cam->lookAt(Vector3(0, 0, -1).norm(), Vector3(0, 1, 0).norm());

	ParticleEmitter *emitter = new ParticleEmitter(100);
	Node *emitterNode = new Node(Vector3::VEC_ZERO, Quaternion(-1.57, Vector3(1, 0, 0)));

	Material *mat = new Material(PATH + "particle");
	string images[] = {TEX_PATH + "smoke00.png"};
	AssetManager::getSingleton()->load(images[0]);
	Texture *texture = new Texture(images, 1, false);
	mat->addTexUniform("tex", texture, false);

	/*
	 * Sets emitter properties, such as the color gradient of the
	 * grayscale particle image, particle lifetime in seconds and
	 * their speed
	*/
	emitter->setMaterial(mat);
	emitter->setStartColor(Vector4(1, 1, 1, 1));
	emitter->setEndColor(Vector4(1, 0, 1, 1));
	emitter->setLowLife(.3);
	emitter->setHighLife(.4);
	emitter->setSpeed(.01);

	vector<Keyframe> sprKeyfrm = vector<Keyframe>{
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 360, 360),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 450, 450)
	};
	KeyframeChannel sprChan = KeyframeChannel::createKeyframeChannel(KeyframeChannel::PARTICLE_EMITTER_SPREAD, emitter->getName(), sprKeyfrm);

	vector<Keyframe> sizeKeyfrm = vector<Keyframe>{
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .05, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .05, 300),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .3, 450)
	};
	KeyframeChannel sizeXChan = KeyframeChannel::createKeyframeChannel(KeyframeChannel::PARTICLE_EMITTER_SIZE_X, emitter->getName(), sizeKeyfrm);
	KeyframeChannel sizeYChan = KeyframeChannel::createKeyframeChannel(KeyframeChannel::PARTICLE_EMITTER_SIZE_Y, emitter->getName(), sizeKeyfrm);

	vector<Keyframe> redKeyfrm = vector<Keyframe>{
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 450),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, .1, 600)
	};
	KeyframeChannel startRedChan = KeyframeChannel::createKeyframeChannel(KeyframeChannel::PARTICLE_EMITTER_START_COLOR_X, emitter->getName(), redKeyfrm);
	KeyframeChannel endRedChan = KeyframeChannel::createKeyframeChannel(KeyframeChannel::PARTICLE_EMITTER_END_COLOR_X, emitter->getName(), redKeyfrm);

	string animName = "anim";
	Animation *animation = new Animation(animName);
	animation->addKeyframeChannel(sprChan);
	animation->addKeyframeChannel(sizeXChan);
	animation->addKeyframeChannel(sizeYChan);
	animation->addKeyframeChannel(startRedChan);
	animation->addKeyframeChannel(endRedChan);

	/*
	 * The AnimationController plays the animations set in the AnimationChannel objects.
	 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
	*/
	AnimationController *controller = AnimationController::getSingleton();
	controller->addAnimation(animation);
	AnimationChannel *animChan = new AnimationChannel();
	controller->addAnimationChannel(animChan);
	animChan->addAnimatable(emitter);
	animChan->setAnimationName(animName);
	animChan->setLoop(true);

	emitterNode->attachParticleEmitter(emitter);
	rootNode->attachChild(emitterNode);

	while(true)
		root->update();

	return 0;
}
