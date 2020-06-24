#include"stb_image.h"
#include"root.h"
#include"node.h"
#include"shader.h"
#include"box.h"
#include"quad.h"
#include<glad.h>
#include<glfw3.h>
#include<cstdlib>
#include<iostream>

using namespace std;

namespace vb01{
	static Root *root=nullptr;

	Root* Root::getSingleton(){
		if(!root)
			root=new Root();
		return root;
	}

	void foo(GLFWwindow *window, int width, int height){
		glViewport(0,0,width,height);
	}

	Root::Root(){
		rootNode=new Node(Vector3::VEC_ZERO);
		guiNode=new Node(Vector3::VEC_ZERO);

		camera=new Camera();
	}

	void Root::start(int width,int height,string name){
		this->width=width;
		this->height=height;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		window=glfwCreateWindow(width,height,name.c_str(),NULL,NULL);
		if(window==NULL){
			std::cout<<"Failed to load window.\n";
			exit(-1);
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window,foo);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			std::cout<<"Failed to load GLAD.\n";
			exit(-1);
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		glGenFramebuffers(1,&FBO);
		glBindFramebuffer(GL_FRAMEBUFFER,FBO);
		Texture *fragTexture=new Texture(width,height,false);
		Texture *brightTexture=new Texture(width,height,false);
		for(int i=0;i<2;i++){
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,*((i==0?fragTexture:brightTexture)->getTexture()),0);
		}
		u32 colorAttachments[]{GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2,colorAttachments);

		glGenRenderbuffers(1,&RBO);
		glBindRenderbuffer(GL_RENDERBUFFER,RBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);
		/*
		*/
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			cout<<"Not complete\n";
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		guiPlane=new Quad(Vector3(width,height,-1),false);
		Material *mat=new Material(Material::MATERIAL_POST);
		mat->addDiffuseMap(fragTexture);
		mat->addDiffuseMap(brightTexture);
		guiPlane->setMaterial(mat);

		string basePath="../../vb01/blur.";
		blurShader=new Shader(basePath+"vert",basePath+"frag");
		glGenFramebuffers(2,pingpongBuffers);
		for(int i=0;i<2;i++){
			glBindFramebuffer(GL_FRAMEBUFFER,pingpongBuffers[i]);
			pingPongTextures[i]=new Texture(width,height,false);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,*(pingPongTextures[i]->getTexture()),0);
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
				cout<<"Not complete\n";
		}
	}

	void Root::update(){
		glBindFramebuffer(GL_FRAMEBUFFER,FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		if(skybox){
			glDepthMask(GL_FALSE);
			//glCullFace(GL_FRONT);
			skybox->update();
			glDepthMask(GL_TRUE);
			//glCullFace(GL_BACK);
		}

		rootNode->update();

		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		Material *material=guiPlane->getMaterial();
		Shader *shader=material->getShader();

		bool horizontal=false;
		blurShader->use();
		blurShader->setVec2(Vector2(width,height),"screen");
		for(int i=0;i<blurLevel;i++){
			glBindFramebuffer(GL_FRAMEBUFFER,pingpongBuffers[horizontal]);
			blurShader->setBool(horizontal,"horizontal");
			if(i==0)
				glBindTexture(GL_TEXTURE_2D,*(material->getDiffuseMap(1)->getTexture()));
			else
				pingPongTextures[!horizontal]->select();
			guiPlane->render();
			horizontal=!horizontal;
		}
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		shader->use();
		shader->setVec2(Vector2(width,height),"screen");
		shader->setBool(bloom,"bloom");
		shader->setBool(hdr,"hdr");
		shader->setFloat(exposure,"exposure");
		shader->setFloat(gamma,"gamma");
		material->getDiffuseMap(0)->select();
		pingPongTextures[!horizontal]->select(1);
		shader->setInt(0,"frag");
		shader->setInt(1,"bright");
		guiPlane->render();

		glEnable(GL_DEPTH_TEST);
		guiNode->update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Root::createSkybox(string textures[6]){
		skybox = new Box(Vector3(10,10,10));
		Material *skyboxMat=new Material(Material::MATERIAL_SKYBOX);
		skyboxMat->addDiffuseMap(textures);
		skybox->setMaterial(skyboxMat);
	}

	void Root::removeSkybox(){
		Material *mat=skybox->getMaterial();
		delete skybox;
		skybox=nullptr;
	}
}
