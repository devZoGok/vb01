#ifndef ROOT_H
#define ROOT_H

#include"node.h"
#include"camera.h"

class GLFWwindow;

namespace vb01{
	void foo();

	class Root{
		public:
			Root();
			void update();
			void start(int,int);
			inline Camera* getCamera(){return camera;}
			inline Node* getRootNode(){return rootNode;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			static Root* getSingleton();
		private:
			bool running=false;
			int width,height;
			GLFWwindow *window;
			Node *rootNode;
			Camera *camera;
			void framebuffer_size_callback(GLFWwindow*,int,int);
	};
}

#endif
