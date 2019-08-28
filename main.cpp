#include<iostream>
#include"root.h"
#include"node.h"
#include"quad.h"
#include"material.h"

using namespace vb01;
using namespace std;

int main(){
	Root *root=Root::getSingleton();
	root->start(800,600);

	Quad *q=new Quad(Vector3(.5,.5,.5));
	Node *node=new Node(Vector3(0,0,2));
	node->setObject(q);
	Material *mat=new Material();
	mat->setDiffuseMap("/home/dominykas/c++/FSim/woodChips.jpg");	
	mat->setLightingEnabled(true);
	q->setMaterial(mat);
	root->getRootNode()->attachChild(node);
	while(true)
		root->update();
	return 0;
}
