#include"model.h"
#include"light.h"
#include"node.h"
#include"root.h"
#include"shader.h"
#include"material.h"
#include"mesh.h"
#include<glad.h>
#include<glfw3.h>
#include<glm.hpp>
#include<ext.hpp>
#include<iostream>

using namespace glm;
using namespace std;

namespace vb01{
	Light::Light(){
		glGenFramebuffers(1,&depthmapFBO);

		depthMap=new Texture(depthMapSize,depthMapSize);

		glBindFramebuffer(GL_FRAMEBUFFER,depthmapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,*(depthMap->getTexture()),0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			cout<<"Not complete\n";
		else 
			cout<<"Complete\n";

		string basePath="../../vb01/depthMap.";
		depthMapShader=new Shader(basePath+"vert",basePath+"frag");
	}

	Light::~Light(){
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glDeleteFramebuffers(1,&depthmapFBO);

		delete depthMap;

		delete depthMapShader;
	}

	void Light::update(){
		Root *root=Root::getSingleton();
		Node *rootNode=root->getRootNode();
		Camera *cam=root->getCamera();
		std::vector<Node*> descendants;
		rootNode->getDescendants(rootNode,descendants);
		descendants.push_back(rootNode);
		std::vector<Material*> materials;
		int numLights=0,thisId=-1;

		vec3 dir=vec3(direction.x,direction.y,direction.z);
		vec3 pos=type==DIRECTIONAL?-.5f*farPlane*dir:vec3(position.x,position.y,position.z);
		mat4 view=lookAt(pos,pos+dir,vec3(0,1,0));
		mat4 proj=ortho(-10.f,10.f,-10.f,10.f,nearPlane,farPlane);

		depthMapShader->use();
		depthMapShader->setMat4(proj*view,"lightMat");

		glViewport(0,0,depthMapSize,depthMapSize);
		glBindFramebuffer(GL_FRAMEBUFFER,depthmapFBO);
		for(Node *n : descendants){
			std::vector<Light*> lights=n->getLights();
			vector<Mesh*> meshes=n->getMeshes();
			for(Mesh *mesh : meshes){
				Material *mat=mesh->getMaterial();
				if(mat&&mat->isLightingEnabled()){
					materials.push_back(mat);
					if(mesh->isCastShadow()){
						Vector3 pos=n->getPosition();
						mat4 model=translate(mat4(1.f),vec3(pos.x,pos.y,pos.z));
						depthMapShader->setMat4(model,"model");
						mesh->render();
					}
				}
			}
			for(Light *l : lights){
				if(l){
					numLights++;
					if(l==this)
						thisId=numLights-1;
				}	
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		glViewport(0,0,root->getWidth(),root->getHeight());

		for(Material *m : materials){
			Shader *shader=m->getShader();
			shader->use();
			shader->setInt((int)type,"light["+to_string(thisId)+"].type");
			shader->setVec3(color,"light["+to_string(thisId)+"].color");
			shader->setInt(0,"tex");
			shader->setInt(thisId+1,"light["+to_string(thisId)+"].depthMap");
			depthMap->select(thisId+1);

			switch(type){
				case POINT:
					shader->setVec3(position,"light["+to_string(thisId)+"].pos");
					shader->setFloat(attenuationValues.x,"light["+to_string(thisId)+"].a");
					shader->setFloat(attenuationValues.y,"light["+to_string(thisId)+"].b");
					shader->setFloat(attenuationValues.z,"light["+to_string(thisId)+"].c");
					break;
				case DIRECTIONAL:
					shader->setMat4(proj*view,"light["+to_string(thisId)+"].lightMat");
					shader->setVec3(direction,"light["+to_string(thisId)+"].direction");
					break;
				case SPOT:
					shader->setMat4(proj*view,"light["+to_string(thisId)+"].lightMat");
					shader->setVec3(position,"light["+to_string(thisId)+"].pos");
					shader->setVec3(direction,"light["+to_string(thisId)+"].direction");
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
