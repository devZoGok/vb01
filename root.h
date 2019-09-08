#ifndef ROOT_H
#define ROOT_H

#include"node.h"
#include"camera.h"
#include<string>

class GLFWwindow;

namespace vb01{
	void foo();

	class Mesh;
	class Box;
	class Shader;

	class Root{
		public:
			Root();
			void update();
			void start(int,int);
			inline Camera* getCamera(){return camera;}
			inline Node* getRootNode(){return rootNode;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			void createSkybox(std::string[6]);
			static Root* getSingleton();
		private:
			Box *skybox=nullptr;
			bool running=false;
			int width,height;
			GLFWwindow *window;
			Node *rootNode;
			Camera *camera;
			std::vector<Mesh*> meshes;
			void framebuffer_size_callback(GLFWwindow*,int,int);
	};
}

#endif
