#include"stb_image.h"
#include"root.h"
#include<glad.h>
#include<glfw3.h>
#include<cstdlib>
#include<iostream>
#include"shader.h"
#include"box.h"
#include"quad.h"

using namespace std;

namespace vb01{
	static Root *root=new Root();

	Root* Root::getSingleton(){return root;}

	void foo(GLFWwindow *window, int width, int height){
		glViewport(0,0,width,height);
	}

	Root::Root(){
		rootNode=new Node(Vector3::VEC_ZERO);
		guiNode=new Node(Vector3::VEC_ZERO);

		camera=new Camera();
	}

	void Root::start(int width,int height){
		running=true;
		this->width=width;
		this->height=height;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		window=glfwCreateWindow(width,height,"KEK",NULL,NULL);
		if(window==NULL){
			std::cout<<"KEK\n";
			exit(-1);
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window,foo);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			std::cout<<"KEK2\n";
			exit(-1);
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glGenFramebuffers(1,&guiFBO);
		glBindFramebuffer(GL_FRAMEBUFFER,guiFBO);
		Texture *texture=new Texture();
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,*(texture->getTexture()),0);

		glGenRenderbuffers(1,&guiRBO);
		glBindRenderbuffer(GL_RENDERBUFFER,guiRBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,guiRBO);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			cout<<"Not complete\n";
		else 
			cout<<"Complete\n";
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		guiPlane=new Quad(Vector3::VEC_IJK);
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->addDiffuseMap(texture);
		guiPlane->setMaterial(mat);
	}

	void Root::update(){
		if(running){
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);

			if(skybox){
				glDepthMask(GL_FALSE);
				glCullFace(GL_BACK);
				skybox->update();
				glDepthMask(GL_TRUE);
				glCullFace(GL_FRONT);
			}

			rootNode->update();
			glDisable(GL_CULL_FACE);

			guiNode->update();
			glBindFramebuffer(GL_FRAMEBUFFER,guiFBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER,0);
			
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//guiPlane->update();
			glDisable(GL_DEPTH_TEST);

			glfwSwapBuffers(window);
		}
	}

	void Root::createSkybox(string textures[6]){
		skybox = new Box(Vector3(10,10,10));
		Material *skyboxMat=new Material(Material::Type::MATERIAL_SKYBOX);
		skyboxMat->addDiffuseMap(textures);
		skybox->setMaterial(skyboxMat);
	}
}
