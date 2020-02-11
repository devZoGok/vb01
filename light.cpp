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
		std::vector<Material*> materials;
		rootNode->getDescendants(rootNode,descendants);
		descendants.push_back(rootNode);
		int numLights=0,thisId=-1;
		float fov=cam->getFov(),width=root->getWidth(),height=root->getHeight();

		for(Node *d : descendants){
			std::vector<Light*> lights=d->getLights();
			for(Light *l : lights){
				if(l){
					numLights++;
					if(l==this)
						thisId=numLights-1;
				}	
			}
			for(Mesh *m : d->getMeshes())
				materials.push_back(m->getMaterial());
		}
		for(int i=0;i<descendants.size();i++){
			Node *n=descendants[i];
			vector<Mesh*> meshes=n->getMeshes();
			for(Mesh *mesh : meshes){
				Material *mat=mesh->getMaterial();
				if(n->isVisible()&&mat&&mat->isLightingEnabled()){
					mat4 proj,view;
					if(mesh->isCastShadow()){
						Vector3 pos=n->getWorldPosition();
						Quaternion rot=n->getWorldOrientation();
						Vector3 rotAxis=rot.norm().getAxis();

						if(rotAxis==Vector3::VEC_ZERO)
							rotAxis=Vector3::VEC_I;

						glViewport(0,0,depthMapSize,depthMapSize);
						glBindFramebuffer(GL_FRAMEBUFFER,depthmapFBO);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

						Vector3 upDir=direction.cross(Vector3(0,1,0));
						if(upDir==Vector3(0,0,0))
							upDir=Vector3::VEC_I;

						vec3 dir=vec3(direction.x,direction.y,direction.z);
						vec3 up=vec3(upDir.x,upDir.y,upDir.z);
						vec3 p=type==DIRECTIONAL?vec3(pos.x,pos.y,pos.z)-.5f*farPlane*dir:vec3(position.x,position.y,position.z);
						view=lookAt(vec3(0,30,0),vec3(0,0,0),vec3(1,0,0));
						proj=ortho(-10.f,10.f,-10.f,10.f,nearPlane,farPlane);

						depthMapShader->use();
						depthMapShader->setMat4(proj*view,"lightMat");

						mat4 model=translate(mat4(1.f),vec3(pos.x,pos.y,pos.z));
						model=rotate(model,rot.getAngle(),vec3(rotAxis.x,rotAxis.y,rotAxis.z));
						depthMapShader->setMat4(model,"model");
						mesh->render();

						glBindFramebuffer(GL_FRAMEBUFFER,*(root->getFBO()));
						glViewport(0,0,root->getWidth(),root->getHeight());

						/*
						for(Material *m : materials){
							Shader *shader=m->getShader();
							shader->use();
							shader->setInt(1+2*thisId+i,"light["+to_string(thisId)+"].depthMap["+to_string(i)+"]");
							depthMap->select(1+2*thisId+i);
						}
						*/
					}
					for(Material *m : materials){
						Shader *shader=m->getShader();
						shader->use();
						shader->setInt((int)type,"light["+to_string(thisId)+"].type");
						shader->setVec3(color,"light["+to_string(thisId)+"].color");
						shader->setInt(0,"tex");
						shader->setInt(1,"light["+to_string(thisId)+"].depthMap["+to_string(0)+"]");
						depthMap->select(1);

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
		}
	}
}
