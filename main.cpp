#include<cmath>
#include<iostream>
#include<string>
#include"root.h"
#include"node.h"
#include"quad.h"
#include"box.h"
#include"material.h"
#include"model.h"
#include"light.h"
#include<glm.hpp>
#include"particleEmitter.h"

using namespace vb01;
using namespace std;

int main(){
	Root *root=Root::getSingleton();
	root->start(800,600);

	Vector3 v1[]={Vector3(.2,.2,.2),Vector3(1,1,1)};
	Vector3 v2[]={Vector3(2,0,0),Vector3(-1,0,0)};

	string t[]={
		"/home/dominykas/c++/FSim/left.jpg",
		"/home/dominykas/c++/FSim/right.jpg",
		"/home/dominykas/c++/FSim/top.jpg",
		"/home/dominykas/c++/FSim/bottom.jpg",
		"/home/dominykas/c++/FSim/front.jpg",
		"/home/dominykas/c++/FSim/back.jpg"
	};
	root->createSkybox(t);

	for(int i=0;i<2;i++){
	Box *box=new Box(Vector3(1,1,1));
	Node *node=new Node(v2[i]);
	Material *mat=new Material();
	mat->addDiffuseMap("/home/dominykas/c++/FSim/defaultTexture.jpg");
	mat->setLightingEnabled(false);
	box->setMaterial(mat);
	node->attachMesh(box);
	//root->getRootNode()->attachChild(node);
	}

	ParticleEmitter *pe=new ParticleEmitter(100);
	Node *node2=new Node(Vector3(0,0,0));
	Material *mat2=new Material(Material::MATERIAL_PARTICLE);
	mat2->addDiffuseMap("/home/dominykas/c++/FSim/stern.jpg");
	pe->setMaterial(mat2);
	node2->attachParticleEmitter(pe);
	root->getRootNode()->attachChild(node2);

	Camera *cam=Root::getSingleton()->getCamera();
	cam->setPosition(Vector3(0,0,-5));
	Light *l1=new Light();
	l1->setPosition(Vector3(-.5,.1,0));
	l1->setColor(Vector3(0,1,0));
	l1->setAttenuationValues(.07,.14,1);
	l1->setDirection(Vector3(0,-1,0));
	l1->setInnerAngle(glm::radians(40.));
	l1->setOuterAngle(glm::radians(60.));
	l1->setType(Light::POINT);
	//root->getRootNode()->addLight(l1);
	while(true){
		root->update();
	}
	return 0;
}
