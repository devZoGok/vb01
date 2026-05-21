#ifndef ROOT_H
#define ROOT_H

#include "camera.h"
#include "util.h"

#include <string>
#include <vector>

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
			void start(int, int, std::string, std::string);
			void toggleHDR(bool);
			void toggleBloom(bool);
			inline Camera* getCamera(){return camera;}
			inline Node* getRootNode(){return rootNode;}
			inline Node* getGuiNode(){return guiNode;}
			inline int getWidth(){return width;}
			inline int getHeight(){return height;}
			inline u32* getFBO(){return &FBO;}
			inline u32* getRBO(){return &RBO;}
			inline GLFWwindow* getWindow(){return window;}
			inline void setExposure(float exposure){this->exposure = exposure;}
			inline void setGamma(float gamma){this->gamma = gamma;}
			inline void setBlurLevel(bool level){this->blurLevel = level;}
			inline Box* getSkybox(){return skybox;}
			inline Box* getIblBox(){return iblBox;}
			inline Quad* getBrdfLutPlane(){return brdfLutPlane;}
			void createSkybox(std::string[6]);
			void createSkybox(vb01::Texture*);
			void removeSkybox();
			static Root* getSingleton();
			inline void shiftNumLights(bool increase){numLights += (increase ? 1 : -1);}
			inline int getNumLights(){return numLights;}
			inline std::string getLibPath(){return libPath;}
		private:
			int numLights = 0;
			bool bloom = false, hdr = false;
			float exposure = 1, gamma = 1;
			Box *skybox = nullptr, *iblBox = nullptr;
			Quad *guiPlane = nullptr, *brdfLutPlane = nullptr;
			int width, height, blurLevel = 10; 
			u32 FBO, RBO, pingpongBuffers[2];
			GLFWwindow *window;
			Node *rootNode, *guiNode;
			Camera *camera;
			Shader *blurShader;
			Texture *pingPongTextures[2];
			std::vector<Mesh*> meshes;
			std::string libPath;

			void framebuffer_size_callback(GLFWwindow*, int, int);
			Root();
			void initWindow(std::string);
			void initMainFramebuffer(Texture*, Texture*);
			void initBloomFramebuffer();
			void initGuiPlane(Texture*, Texture*);
			void updateBloomFramebuffer();
			void updateGuiPlane();
			void updateNodeTree(vb01::Node*, bool);
	};
}

#endif
