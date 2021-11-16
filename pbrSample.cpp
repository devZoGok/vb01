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

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(0, 1, 1) * 7);
	cam->lookAt(Vector3(0, -1, -1).norm(), Vector3(0, 1, -1).norm());

	Node *rootNode = root->getRootNode();

	Model *model = new Model(MODEL_PATH + "teapot.vb");
	Material *mat = new Material(PATH + "pbr");
	mat->addBoolUniform("albedoMapEnabled", true);
	mat->addBoolUniform("normalMapEnabled", true);
	mat->addBoolUniform("roughnessMapEnabled", true);
	mat->addBoolUniform("metallnessMapEnabled", true);
	string fr0[]{TEX_PATH + "geyser-rock1_albedo.jpg"};
	string fr1[]{TEX_PATH + "geyser-rock1_normal.jpg"};
	string fr2[]{TEX_PATH + "geyser-rock1_roughness.jpg"};
	string fr3[]{TEX_PATH + "geyser-rock1_metallic.jpg"};
	string fr4[]{TEX_PATH + "geyser-rock1_ao.jpg"};
	mat->addTexUniform("textures[0]", new Texture(fr0, 1), true);
	mat->addTexUniform("textures[1]", new Texture(fr1, 1), true);
	mat->addTexUniform("textures[2]", new Texture(fr2, 1), true);
	mat->addTexUniform("textures[3]", new Texture(fr3, 1), true);
	mat->addTexUniform("textures[4]", new Texture(fr4, 1), true);
	model->setMaterial(mat);
	rootNode->attachChild(model);

	{
	Light *light = new Light(Light::POINT);
	light->setColor(Vector3(10, 0, 0));
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
	lightNode->setPosition(Vector3(2, 3, 2));
	lightNode->setOrientation(Quaternion(1.57, Vector3::VEC_I));
	}
	{
	Light *light = new Light(Light::POINT);
	light->setColor(Vector3(0, 0, 10));
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
