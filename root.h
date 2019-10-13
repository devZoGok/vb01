#ifndef ROOT_H
#define ROOT_H

#include"camera.h"
#include<string>
#include<vector>

class GLFWwindow;

namespace vb01{
	void foo();

	class Mesh;
	class Box;
	class Quad;
	class Shader;
	class Node;

	class Root{
		public:
			void update();
			void start(int,int);
			inline Camera* getCamera(){return camera;}
			inline Node* getRootNode(){return rootNode;}
			inline Node* getGuiNode(){return guiNode;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			inline GLFWwindow* getWindow(){return window;}
			void createSkybox(std::string[6]);
			static Root* getSingleton();
		private:
			Box *skybox=nullptr;
			Quad *guiPlane=nullptr;
			int width,height;
			unsigned int FBO,RBO;
			GLFWwindow *window;
			Node *rootNode,*guiNode;
			Camera *camera;
			std::vector<Mesh*> meshes;

			void framebuffer_size_callback(GLFWwindow*,int,int);
			Root();
	};
}

#endif
