#include "root.h"
#include "node.h"
#include "particleEmitter.h"
#include "material.h"
#include "texture.h"
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
	root->start(800, 600, PATH, "Particle emitter sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 0, 1) * 1);
	cam->lookAt(Vector3(0, 0, -1).norm(), Vector3(0, 1, 0).norm());

	ParticleEmitter *emitter = new ParticleEmitter(1);
	Node *emitterNode = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "", new AnimationController());

	Material *mat = new Material(Material::MATERIAL_PARTICLE);
	mat->setTexturingEnabled(true);
	mat->setLightingEnabled(false);
	string images[] = {TEX_PATH + "smoke00.png"};
	Texture *texture = new Texture(images, 1);
	mat->addDiffuseMap(texture);

	emitter->setMaterial(mat);
	emitter->setStartColor(Vector4(1, 1, 1, 1));
	emitter->setEndColor(Vector4(1, 0, 1, 1));
	emitter->setStartSize(Vector2(1, 1) * .1);
	emitter->setEndSize(Vector2(1, 1) * .1);
	emitter->setLowLife(.3);
	emitter->setHighLife(.4);
	emitter->setSpeed(.01);
	emitter->setSpread(360);

	emitterNode->attachParticleEmitter(emitter);
	rootNode->attachChild(emitterNode);
	emitterNode->setOrientation(Quaternion(-1.57, Vector3(1, 0, 0)));

	while(true)
		root->update();
	return 0;
}
