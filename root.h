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
	class Node;
	class Texture;
	class Shader;

	class Root{
		public:
			void update();
			void start(int,int);
			inline Camera* getCamera(){return camera;}
			inline Node* getRootNode(){return rootNode;}
			inline Node* getGuiNode(){return guiNode;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			inline unsigned int* getFBO(){return &FBO;}
			inline GLFWwindow* getWindow(){return window;}
			inline void setBloom(bool bloom){this->bloom=bloom;}
			void createSkybox(std::string[6]);
			static Root* getSingleton();
			int numLights=0;
		private:
			bool bloom=false;
			Box *skybox=nullptr;
			Quad *guiPlane=nullptr;
			int width,height;
			unsigned int FBO,RBO,pingpongBuffers[2];
			GLFWwindow *window;
			Node *rootNode,*guiNode;
			Camera *camera;
			Shader *blurShader;
			Texture *pingPongTextures[2];
			std::vector<Mesh*> meshes;

			void framebuffer_size_callback(GLFWwindow*,int,int);
			Root();
	};
}

#endif
