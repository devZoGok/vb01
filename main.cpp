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

using namespace vb01;
using namespace std;

int main(){
	Root *root=Root::getSingleton();
	root->start(800,600);

	Vector3 v1[]={Vector3(.2,.2,.2),Vector3(1,1,1)};
	Vector3 v2[]={Vector3(0,0,0),Vector3(.0,0,0)};

	string t[]={
		"/home/dominykas/c++/FSim/left.jpg",
		"/home/dominykas/c++/FSim/right.jpg",
		"/home/dominykas/c++/FSim/top.jpg",
		"/home/dominykas/c++/FSim/bottom.jpg",
		"/home/dominykas/c++/FSim/front.jpg",
		"/home/dominykas/c++/FSim/back.jpg"
	};
	root->createSkybox(t);

	Box *b=new Box(Vector3(1,1,1));
	Node *n=new Node(v2[0]);
	Material *mat=new Material();
	mat->addDiffuseMap("/home/dominykas/c++/FSim/defaultTexture.jpg");
	mat->setLightingEnabled(false);
	b->setMaterial(mat);
	n->attachMesh(b);
	root->getRootNode()->attachChild(n);

	Camera *cam=Root::getSingleton()->getCamera();
	cam->setPosition(Vector3(0,0,-5));
	cam->lookAt(Vector3(1,0,1).norm(),Vector3(0,1,0));
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
