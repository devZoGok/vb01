#ifndef ROOT_H
#define ROOT_H

#include "util.h"
#include "meshData.h"

#include <glm.hpp>

#include <glad.h>

#include <glfw3.h>

#include <unordered_map>
#include <string>
#include <vector>

class GLFWwindow;

namespace vb01{
	void foo();

	class Camera;
	class Mesh;
	class Box;
	class Quad;
	class Node;
	class Texture;
	class Shader;
	class ParticleEmitter;

	struct ImageAsset;

	class Root{
		public:
			static Root* getSingleton();
			void update();
			void start(int, int, std::string, std::string);
			void toggleHDR(bool);
			void toggleBloom(bool);
			void addMeshes(Node*);
			void createSkybox(std::string[6]);
			void createSkybox(vb01::Texture*);
			void removeSkybox();
			void initVertexDataOnGpu(MeshData&, u32&, u32&, u32&, bool);
			void initTextureDataOnGpu(int, int);
			void initTextureDataOnGpu(u32*, u8*, int, int, int&);
			inline Shader* getPhongShader(){return phongShader;}
			inline u32& getMeshVAO(){return meshVAO;}
			inline u32& getMeshVBO(){return meshVBO;}
			inline u32& getMeshEBO(){return meshEBO;}
			inline u32* getMeshTextureBuffer(){return meshTextureBuffer;}
			inline u32* getGuiPlaneTextureBuffer(){return guiPlaneTextureBuffer;}
			inline const int getMaxNumShapeKeys(){return MAX_NUM_SHAPE_KEYS;}
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
			inline void shiftNumLights(bool increase){numLights += (increase ? 1 : -1);}
			inline int getNumLights(){return numLights;}
			inline std::string getLibPath(){return libPath;}
		private:
			struct DrawElementsIndirectCommand {
				u32 count = 0;
				u32 instanceCount = 0;
				u32 firstIndex = 0;
				int baseVertex = 0;
				u32 baseInstance = 0;
			};
			struct ObjectVertexData {
				float shapeKeyFactors[24];
				glm::mat4 viewProj, model, bones[256];
				//bool animated;
			};
			struct ObjectFragmentData {
				float mixRatio;
				int pastTexture[2], lastTexture[2];
				int animated;
				float diffuseColor[4], specularColor[4];
				float shinyness, specularStrength;
				int lightingEnabled, constLightingEnabled, texturingEnabled, normalMapEnabled, specularMapEnabled, castShadow, environmentMapEnabled;
			};
			struct TextureGpuData {
				u32 *buffer = nullptr;
				int unitId = 0, layerId = 0, width = 0, height = 0;

				TextureGpuData(u32 *b, int u, int l, int w, int h) : buffer(b), unitId(u), layerId(l), width(w), height(h){}
			};

			const int MAX_NUM_SHAPE_KEYS = 5;
			int NUM_MAX_TEXTURE_UNITS = 0, NUM_MAX_TEXTURE_ARRAY_SIZE = 0, numLights = 0, width, height, blurLevel = 10, currNumVerts = 0, currNumIndexes = 0, depthmapSize = 512, currNumLayers = 0;
			u32 meshVAO, meshVBO, meshEBO, particleVAO, particleVBO, guiPlaneVAO, guiPlaneVBO, guiPlaneEBO, *meshTextureBuffer = nullptr, *guiPlaneTextureBuffer = nullptr, *depthmapBuffer = nullptr, FBO, RBO, pingpongBuffers[2], drawCmdBuffer = -1, objVertBuffer = -1, objFragBuffer = -1;
			bool bloom = false, hdr = false;
			float exposure = 1, gamma = 1;
			Box *skybox = nullptr, *iblBox = nullptr;
			Quad *guiPlane = nullptr, *brdfLutPlane = nullptr;
			GLFWwindow *window;
			Node *rootNode, *guiNode;
			Camera *camera;
			Shader *blurShader = nullptr, *phongShader = nullptr;
			Texture *pingPongTextures[2];
			std::vector<MeshData*> drawCmdMeshes;
			std::vector<MeshData::GpuVertex> currentGpuVertices;
			std::vector<std::pair<int, int>> currTextureDims;
			std::vector<u32> currentIndices;
			std::vector<Mesh*> meshes;
			std::vector<DrawElementsIndirectCommand> drawCmds;
			std::vector<ObjectVertexData> objVertData;
			std::vector<ObjectFragmentData> objFragData;
			std::vector<TextureGpuData> textureGpuData;
			std::string libPath;

			Root();
			void framebuffer_size_callback(GLFWwindow*, int, int);
			void renderMeshes();
			void renderParticles(std::vector<ParticleEmitter*>&);
			void renderGui(std::vector<Mesh*>&);
			void initWindow(std::string);
			void initMainFramebuffer(Texture*, Texture*);
			void initBloomFramebuffer();
			void initGuiPlane();
			void updateBloomFramebuffer();
			void updateGuiPlane();
			void updateNodeTree(vb01::Node*, glm::mat4&, bool);
			void renderMesh(Mesh*, u32&);

		protected:
			virtual void initMeshRendering(u32 &vao, u32 &vbo, u32 &ebo);
			virtual void initParticleRendering();
			virtual void initGuiRendering();
			virtual void initPostProcessing();
			virtual void postProcess(){}
	};
}

#endif
