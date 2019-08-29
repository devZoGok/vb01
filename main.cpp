#include<iostream>
#include"root.h"
#include"node.h"
#include"quad.h"
#include"material.h"
#include"light.h"
#include<glm.hpp>

using namespace vb01;
using namespace std;

int main(){
	Root *root=Root::getSingleton();
	root->start(800,600);

	Vector3 v1[]={Vector3(2,2,.5),Vector3(.5,.5,.5)};
	Vector3 v2[]={Vector3(-.0,0,2),Vector3(.5,0,-2)};
	for(int i=0;i<1;i++){
		Quad *q=new Quad(v1[i]);
		Node *node=new Node(v2[i]);
		node->setObject(q);
		Material *mat=new Material();
		mat->setDiffuseMap("/home/dominykas/c++/FSim/woodChips.jpg");	
		mat->setLightingEnabled(true);
		q->setMaterial(mat);
		root->getRootNode()->attachChild(node);
	}
	Camera *cam=Root::getSingleton()->getCamera();
	cam->setPosition(Vector3(0,0,-4));
	Light *l1=new Light();
	l1->setPosition(Vector3(-.5,.1,0));
	l1->setColor(Vector3(0,1,0));
	l1->setAttenuationValues(.07,.14,1);
	l1->setDirection(Vector3(0,-1,0));
	l1->setInnerAngle(glm::radians(40.));
	l1->setOuterAngle(glm::radians(60.));
	l1->setType(Light::POINT);
	root->getRootNode()->addLight(l1);
	while(true)
		root->update();
	return 0;
}
