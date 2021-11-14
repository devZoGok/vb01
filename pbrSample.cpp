#include "root.h"
#include "model.h"
#include "material.h"
#include "texture.h"
#include "light.h"
#include "box.h"

#include <string>

using namespace vb01;
using namespace std;

int main() {
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
	root->setHDREnabled(true);

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(1, 1, 1) * 5);
	cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());

	Node *rootNode = root->getRootNode();

	Model *model = new Model(MODEL_PATH + "teapot.vb");
	Material *mat = new Material(PATH + "pbr");
	mat->addBoolUniform("albedoMapEnabled", false);
	mat->addVec4Uniform("albedoColor", Vector4(1, 1, 1, 1));
	mat->addFloatUniform("roughness", .1);
	mat->addFloatUniform("metalness", .1);
	mat->addVec3Uniform("baseReflectivity", Vector3(0.563,  0.579,  0.579));
	/*
	mat = new Material(PATH + "texture");
	mat->addBoolUniform("texturingEnabled", true);
	mat->addBoolUniform("lightingEnabled", true);
	string fr[]{TEX_PATH + "bricks.jpg"};
	mat->addTexUniform("textures[0]", new Texture(fr, 1), true);
	*/
	model->setMaterial(mat);
	rootNode->attachChild(model);

	{
	Light *light = new Light(Light::POINT);
	light->setAttenuationValues(.00001, .00001, 1);
	light->setColor(Vector3(1, 0, 0));
	light->setInnerAngle(.1);
	light->setOuterAngle(.1);
	Node *lightNode = new Node();
	Box *b = new Box(Vector3(1, 1, 1) * .1);
	Material *m = new Material(PATH + "texture");
	m->addBoolUniform("texturingEnabled", false);
	m->addBoolUniform("lightingEnabled", false);
	m->addVec4Uniform("diffuseColor", Vector4(1, 0, 0, 1));
	b->setMaterial(m);
	lightNode->attachMesh(b);
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);
	lightNode->setPosition(Vector3(2, 3, 0));
	lightNode->setOrientation(Quaternion(1.57, Vector3::VEC_I));
	}
	{
	Light *light = new Light(Light::POINT);
	light->setAttenuationValues(.00001, .00001, 1);
	light->setColor(Vector3(0, 0, 1));
	light->setInnerAngle(.1);
	light->setOuterAngle(.1);
	Node *lightNode = new Node();
	Box *b = new Box(Vector3(1, 1, 1) * .1);
	Material *m = new Material(PATH + "texture");
	m->addBoolUniform("texturingEnabled", false);
	m->addBoolUniform("lightingEnabled", false);
	m->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 1));
	b->setMaterial(m);
	lightNode->attachMesh(b);
	lightNode->addLight(light);
	rootNode->attachChild(lightNode);
	lightNode->setPosition(Vector3(-2, 3, 0));
	lightNode->setOrientation(Quaternion(1.57, Vector3::VEC_I));
	}

	while(true)
			root->update();

		return 0;
}
