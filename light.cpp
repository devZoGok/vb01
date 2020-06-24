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
	Light::Light(Type t){
		this->type=t;
		string basePath="../../vb01/depthMap.";

		glGenFramebuffers(1,&depthmapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER,depthmapFBO);
		if(this->type==POINT){
			depthMap=new Texture(depthMapSize);
			glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,*(depthMap->getTexture()),0);
			depthMapShader=new Shader(basePath+"vert",basePath+"frag",basePath+"geo");
		}
		else{
			depthMap=new Texture(depthMapSize,depthMapSize,true);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,*(depthMap->getTexture()),0);
			depthMapShader=new Shader(basePath+"vert",basePath+"frag");
		}
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			cout<<"Not complete\n";
		glBindFramebuffer(GL_FRAMEBUFFER,0);


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
		int thisId=-1;
		float fov=cam->getFov(),width=root->getWidth(),height=root->getHeight();

		for(Node *d : descendants){
			std::vector<Light*> lights=d->getLights();
			for(int i=0;i<lights.size();i++){
				if(lights[i]==this)
					thisId=i;
			}
			for(Mesh *m : d->getMeshes())
				materials.push_back(m->getMaterial());
		}
		vec3 dirs[]{
			vec3(1,0,0),
			vec3(-1,0,0),
			vec3(0,1,0),
			vec3(0,-1,0),
			vec3(0,0,1),
			vec3(0,0,-1)
		};
		mat4 proj=mat4(1.),view=mat4(1.);
		glViewport(0,0,depthMapSize,depthMapSize);
		glBindFramebuffer(GL_FRAMEBUFFER,depthmapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for(Node *n : descendants){
			vector<Mesh*> meshes=n->getMeshes();
			for(Mesh *mesh : meshes){
				Material *mat=mesh->getMaterial();
				if(n->isVisible()&&mat&&mat->isLightingEnabled()){
					Vector3 pos=n->localToGlobalPosition(Vector3::VEC_ZERO);
					Quaternion rot=n->localToGlobalOrientation(Quaternion::QUAT_W);
					Vector3 rotAxis=rot.norm().getAxis();

					if(rotAxis==Vector3::VEC_ZERO)
						rotAxis=Vector3::VEC_I;

					Vector3 upDir=direction.cross(Vector3(0,1,0));
					if(upDir==Vector3(0,0,0))
						upDir=Vector3::VEC_I;

					vec3 dir=vec3(direction.x,direction.y,direction.z);
					vec3 up=vec3(upDir.x,upDir.y,upDir.z);
					vec3 p=type==DIRECTIONAL?vec3(pos.x,pos.y,pos.z)-.5f*farPlane*dir:vec3(position.x,position.y,position.z);
					view=lookAt(p,p+dir,up);
					proj=(type==DIRECTIONAL?ortho(-10.f,10.f,-10.f,10.f,nearPlane,farPlane):perspective(radians(90.f),1.f,nearPlane,farPlane));
					mat4 model=translate(mat4(1.f),vec3(pos.x,pos.y,pos.z));
					model=rotate(model,rot.getAngle(),vec3(rotAxis.x,rotAxis.y,rotAxis.z));

					if(mesh->isCastShadow()){
						depthMapShader->use();
						depthMapShader->setBool(type==POINT,"point");
						depthMapShader->setMat4(model,"model");

						if(type==POINT){
							depthMapShader->setFloat(farPlane,"farPlane");
							depthMapShader->setVec3(position,"lightPos");
							for(int j=0;j<6;j++)
								depthMapShader->setMat4(proj*lookAt(p,p+dirs[j],1<j&&j<4?vec3(0,0,-1):vec3(0,-1,0)),"shadowMat["+to_string(j)+"]");
						}
						else{
							depthMapShader->setMat4(proj*view,"lightMat");
						}
						mesh->render();
					}
				}
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER,*(root->getFBO()));
		glViewport(0,0,root->getWidth(),root->getHeight());

		for(Material *m : materials){
			Shader *shader=m->getShader();
			shader->use();
			shader->setInt((int)type,"light["+to_string(thisId)+"].type");
			shader->setVec3(color,"light["+to_string(thisId)+"].color");
			shader->setFloat(nearPlane,"light["+to_string(thisId)+"].near");
			shader->setFloat(farPlane,"light["+to_string(thisId)+"].far");
			shader->setInt(0,"diffuseMap");
			shader->setInt(1,"normalMap");
			shader->setInt(2,"specularMap");
			shader->setInt(3,"parallaxMap");
			shader->setInt(4,"environmentMap");
			shader->setInt(5,"light["+to_string(thisId)+"].depthMapCube");
			shader->setInt(6,"light["+to_string(thisId)+"].depthMap");
			depthMap->select(type==POINT?5:6);
			/*
			*/

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
