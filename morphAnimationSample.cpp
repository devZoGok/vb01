#include "root.h"
#include "model.h"
#include "material.h"
#include "mesh.h"
#include "skeleton.h"
#include "animationController.h"
#include "animationChannel.h"
#include "box.h"

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
	root->start(800, 600, PATH, "Morph animation sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(-1, 0.25, .2) * 20);
	cam->lookAt(Vector3(1, 0, 0).norm(), Vector3(0, 1, 0).norm());

	Model *model = new Model(MODEL_PATH + "kek.vb");
	Material *mat = new Material();
	mat->setLightingEnabled(false);
	mat->setTexturingEnabled(true);
	string images[] = {TEX_PATH + "defaultTexture.jpg"};
	Texture *texture = new Texture(images, 1);
	mat->addDiffuseMap(texture);
	model->setMaterial(mat);
	rootNode->attachChild(model);

	int numChildren = model->getNumChildren();
	Node *node = model->getChild(1);
	Mesh *mesh = node->getMesh(0);
	Skeleton *skeleton = mesh->getSkeleton();

	AnimationController *controller = skeleton->getAnimationController();
	AnimationChannel *channel = new AnimationChannel();
	controller->addAnimationChannel(channel);
	channel->addAnimatable(skeleton->getBone("bone"));
	channel->setAnimationName("morph");
	channel->setLoop(true);

	Box *box = new Box(Vector3(1, 1, 1) * .25);
	Material *boxMat = new Material();
	boxMat->setTexturingEnabled(false);
	boxMat->setDiffuseColor(Vector4(1, 0, 0, 1));
	box->setMaterial(boxMat);
	skeleton->getBone("bone")->attachMesh(box);

	while(true){
		root->update();
	}

	return 0;
}