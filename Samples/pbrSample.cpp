#include "root.h"
#include "model.h"
#include "material.h"
#include "texture.h"
#include "light.h"
#include "box.h"
#include "quad.h"

#include <string>

using namespace vb01;
using namespace std;

int main() {
	const string PATH = "../", TEX_PATH = PATH + "samples/textures/", MODEL_PATH = PATH + "samples/models/";
	string skyboxTextures[] = {
		TEX_PATH + "bricks.jpg",
		TEX_PATH + "bricks.jpg",
		TEX_PATH + "bricks.jpg",
		TEX_PATH + "bricks.jpg",
		TEX_PATH + "bricks.jpg",
		TEX_PATH + "bricks.jpg"
	};

	/*
	 * Gets Root object to start the sample, 
	 * also passes the base path for asset retrieval. 
	*/
	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Light sample");
	root->createSkybox(skyboxTextures);

	Camera *cam = root->getCamera();
	cam->setPosition(Vector3(1, 1, 1) * 2);
	cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());

	Node *rootNode = root->getRootNode();

	Model *model = new Model(MODEL_PATH + "sphere.vb");
	model->setReflect(true);
	Material *mat = new Material(PATH + "pbr");
	mat->addBoolUniform("albedoMapEnabled", false);
	mat->addBoolUniform("normalMapEnabled", false);
	mat->addBoolUniform("roughnessMapEnabled", false);
	mat->addBoolUniform("metalnessMapEnabled", false);
	mat->addBoolUniform("ambientOcclusionMapEnabled", false);
	mat->addBoolUniform("environmentMapEnabled", true);
	mat->addVec4Uniform("albedoColor", Vector4(1, .764, .03, 1));
	mat->addFloatUniform("metalnessVal", .9);
	mat->addFloatUniform("roughnessVal", .1);
	mat->addFloatUniform("ambientOcclusion", .1);
	/*
	string fr0[]{TEX_PATH + "rustediron2_albedo.jpg"};
	string fr1[]{TEX_PATH + "rustediron2_normal.jpg"};
	string fr2[]{TEX_PATH + "rustediron2_roughness.jpg"};
	string fr3[]{TEX_PATH + "rustediron2_metallic.jpg"};
	string fr4[]{TEX_PATH + "rustediron2_ao.jpg"};
	mat->addTexUniform("textures[0]", new Texture(fr0, 1, false), true);
	mat->addTexUniform("textures[1]", new Texture(fr1, 1, false), true);
	mat->addTexUniform("textures[2]", new Texture(fr2, 1, false), true);
	mat->addTexUniform("textures[3]", new Texture(fr3, 1, false), true);
	mat->addTexUniform("textures[4]", new Texture(fr4, 1, false), true);
	*/
	model->setMaterial(mat);
	rootNode->attachChild(model);
	model->setPosition(Vector3(0, 0, 0));

	{
	Node *lightNode = new Node();
	Box *b = new Box(Vector3(1, 1, 1) * 1);
	Material *m = new Material(PATH + "skybox");
	m->addTexUniform("tex", model->getChild(0)->getMesh(0)->getPostfilterMap(), true);
	b->setMaterial(m);
	lightNode->attachMesh(b);
	rootNode->attachChild(lightNode);
	lightNode->setPosition(Vector3(0, 0, 0));
	lightNode->setOrientation(Quaternion(1.57, Vector3::VEC_I));
	}
	{
	Node *lightNode = new Node();
	Quad *b = new Quad(Vector3(2, 2, 1), false);
	Material *m = new Material(PATH + "texture");
	m->addBoolUniform("texturingEnabled", true);
	m->addBoolUniform("lightingEnabled", false);
	//m->addVec4Uniform("diffuseColor", Vector4(0, 0, 1, 1));
	m->addTexUniform("textures[0]", model->getChild(0)->getMesh(0)->getBrdfIntegrationMap(), true);
	b->setMaterial(m);
	lightNode->attachMesh(b);
	rootNode->attachChild(lightNode);
	lightNode->setPosition(Vector3(-4, 1, 0));
	lightNode->setOrientation(Quaternion(1.57, Vector3::VEC_I));
	}
	{
	Light *light = new Light(Light::POINT);
	light->setColor(Vector3(1, 1, 1) * 10);
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
	lightNode->setPosition(Vector3(0, 4, 3));
	lightNode->setOrientation(Quaternion(1.57, Vector3::VEC_I));
	}
	{
	Light *light = new Light(Light::POINT);
	light->setColor(Vector3(0, 0, 0));
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
