#include"light.h"
#include"node.h"
#include"root.h"
#include"material.h"
#include"mesh.h"
#include<glm/glm.hpp>

using namespace glm;
using namespace std;

namespace vb01{
	Light::Light(){
	
	}

	Light::~Light(){}

	void Light::update(){
		Node *rootNode=Root::getSingleton()->getRootNode();
		std::vector<Node*> descendants=rootNode->getDescendants();
		descendants.push_back(rootNode);
		std::vector<Material*> materials;
		int numLights=0,thisId=-1;
		for(Node *n : descendants){
			std::vector<Light*> lights=n->getLights();
			if(n->getMesh()){
				Material *mat=n->getMesh()->getMaterial();
				if(mat&&mat->isLightingEnabled())
					materials.push_back(mat);
			}
			for(Light *l : lights){
				if(l){
					numLights++;
					if(l==this)
						thisId=numLights-1;
				}	
			}
		}
		for(Material *m : materials){
			Shader *shader=m->getShader();
			shader->use();
			shader->setInt((int)type,"light["+to_string(thisId)+"].type");
			shader->setVec3(vec3(color.x,color.y,color.z),"light["+to_string(thisId)+"].color");
			switch(type){
				case POINT:
					shader->setVec3(vec3(pos.x,pos.y,pos.z),"light["+to_string(thisId)+"].pos");
					shader->setFloat(attenuationValues.x,"light["+to_string(thisId)+"].a");
					shader->setFloat(attenuationValues.y,"light["+to_string(thisId)+"].b");
					shader->setFloat(attenuationValues.z,"light["+to_string(thisId)+"].c");
					break;
				case DIRECTIONAL:
					shader->setVec3(vec3(direction.x,direction.y,direction.z),"light["+to_string(thisId)+"].direction");
					break;
				case SPOT:
					shader->setVec3(vec3(pos.x,pos.y,pos.z),"light["+to_string(thisId)+"].pos");
					shader->setVec3(vec3(direction.x,direction.y,direction.z),"light["+to_string(thisId)+"].direction");
					shader->setFloat(attenuationValues.x,"light["+to_string(thisId)+"].a");
					shader->setFloat(attenuationValues.y,"light["+to_string(thisId)+"].b");
					shader->setFloat(attenuationValues.z,"light["+to_string(thisId)+"].c");
					shader->setFloat(innerAngle,"light["+to_string(thisId)+"].innerAngle");
					shader->setFloat(outerAngle,"light["+to_string(thisId)+"].outerAngle");
					break;
			}
		}
	}
}
