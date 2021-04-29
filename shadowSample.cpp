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

	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Shadow sample");
	root->createSkybox(skyboxTextures);
	Node *rootNode = root->getRootNode();

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 1, 1) * 20);
	cam->lookAt(Vector3(0, -1, -1).norm(), Vector3(0, 1, -1).norm());

	Model *box = new Model(MODEL_PATH + "platform.vb");
	Material *mat = new Material();
	mat->setTexturingEnabled(true);
	mat->setLightingEnabled(true);
	mat->setNormalMapEnabled(false);
	mat->setSpecularMapEnabled(false);
	string im0[] = {TEX_PATH + "bricks.jpg"};
	Texture *diffuseTex = new Texture(im0, 1);
	mat->addDiffuseMap(diffuseTex);
	box->setMaterial(mat);
	rootNode->attachChild(box);

	{
	Model *box = new Model(MODEL_PATH + "jet00.obj");
	Material *mat = new Material();
	mat->setTexturingEnabled(true);
	mat->setLightingEnabled(true);
	mat->setNormalMapEnabled(false);
	mat->setSpecularMapEnabled(false);
	string im0[] = {TEX_PATH + "defaultTexture.jpg"};
	Texture *diffuseTex = new Texture(im0, 1);
	mat->addDiffuseMap(diffuseTex);
	box->setMaterial(mat);
	rootNode->attachChild(box);
	box->setCastShadow(true);
	box->setPosition(Vector3(0, 3, 0));
	}

	Light *light = new Light(Light::DIRECTIONAL);
	light->setColor(Vector3(1, 1, 1));
	Node *lightNode = new Node(Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK, "", new AnimationController());
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);
	lightNode->setOrientation(Quaternion(1.57, Vector3(1, 0, 0)));
	lightNode->setPosition(Vector3(0, 5, -5));

	while(true)
		root->update();

	return 0;
}
