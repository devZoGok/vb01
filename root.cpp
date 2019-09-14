#include"stb_image.h"
#include"root.h"
#include<glad.h>
#include<glfw3.h>
#include<cstdlib>
#include<iostream>
#include"shader.h"
#include"box.h"

using namespace std;

namespace vb01{
	static Root *root=new Root();

	Root* Root::getSingleton(){return root;}

	void foo(GLFWwindow *window, int width, int height){
		glViewport(0,0,width,height);
	}

	Root::Root(){
		rootNode=new Node(Vector3::VEC_ZERO);
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	void Root::update(){
		if(running){
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if(skybox){
				glDepthMask(GL_FALSE);
				glCullFace(GL_BACK);
				skybox->update();
				glDepthMask(GL_TRUE);
				glCullFace(GL_FRONT);
			}
			rootNode->update();

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
