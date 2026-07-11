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
			void createSkybox(std::string[6]);
			void createSkybox(vb01::Texture*);
			void removeSkybox();
			void initVertexDataOnGpu(MeshData&, u32&, u32&, u32&, bool);
			void initTextureDataOnGpu(int, int);
			void initTextureDataOnGpu(u8*, int, int, int&, int&, bool, bool = false, int = 0, int = 0);
			void updateRenderNodeData(Node*);
			void createCubemap(bool, bool, std::string[6], int);
			inline Quad* getGuiQuad(){return guiQuad;}
			inline Shader* getPhongShader(){return phongShader;}
			inline Shader* getGuiShader(){return guiShader;}
			inline u32& getMeshVAO(){return meshVAO;}
			inline u32& getMeshVBO(){return meshVBO;}
			inline u32& getMeshEBO(){return meshEBO;}
			inline u32& getGuiVAO(){return guiVAO;}
			inline u32& getGuiVBO(){return guiVBO;}
			inline u32& getGuiEBO(){return guiEBO;}
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
			struct ObjectIdData{
				int drawId = 0, instanceId = 0;
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
			struct LightData {
				int useAngle, additive, render;
				int type, attenuation;
				float pos[3], color[3], direction[3];
				float innerAngle, outerAngle;
				float a, b, c, near, far, radius;
			};
			struct GuiData {
				float pos[3], scale[3], size[2];
				int texturingEnabled, glyphTexture[2], pastTexture[2], nextTexture[2];
				float diffuseColor[4];
			};
			struct TextureUnitGpuData {
				u32 buffer = -1;
				int numLayers = 0, width = 0, height = 0;
				std::vector<u8*> imageData;
				std::vector<std::pair<int, int>> subDims;

				TextureUnitGpuData(int w, int h) : width(w), height(h){}
			};

			glm::mat4 projView;
			const int MAX_NUM_SHAPE_KEYS = 5;
			int NUM_MAX_TEXTURE_UNITS = 0, NUM_MAX_TEXTURE_ARRAY_SIZE = 0, numLights = 0, width, height, blurLevel = 10, currNumVerts = 0, currNumIndexes = 0, depthmapSize = 512, currNumLayers = 0;
			u32 meshVAO = -1, meshVBO = -1, meshEBO = -1;
			u32 particleVAO = -1, particleVBO = -1; 
			u32 guiVAO = -1, guiVBO = -1, guiEBO = -1;
			u32 guiPlaneVAO = -1, guiPlaneVBO = -1, guiPlaneEBO = -1, *guiPlaneTextureBuffer = nullptr;
			u32 skyboxVAO = -1, skyboxVBO = -1, skyboxEBO = -1, skyboxBuffer = -1;
			u32 FBO = -1, RBO = -1, pingpongBuffers[2], drawCmdBuffer = -1, objVertBuffer = -1, objFragBuffer = -1, objLightBuffer = -1, guiDataBuffer = -1;
			bool bloom = false, hdr = false;
			float exposure = 1, gamma = 1;
			Box *skybox = nullptr, *iblBox = nullptr;
			Quad *guiPlane = nullptr, *brdfLutPlane = nullptr;
			GLFWwindow *window;
			Node *rootNode, *guiNode;
			Camera *camera;
			Shader *blurShader = nullptr, *phongShader = nullptr, *guiShader = nullptr;
			Texture *pingPongTextures[2];
			Quad *guiQuad = nullptr;
			std::vector<MeshData*> drawCmdMeshes;
			std::vector<MeshData::GpuVertex> currentGpuVertices;
			std::vector<std::pair<int, int>> currTextureDims;
			std::vector<u32> currentIndices;
			std::vector<Mesh*> meshes;
			std::vector<DrawElementsIndirectCommand> drawCmds, drawCmds2;
			std::vector<ObjectIdData> objIdData;
			std::vector<ObjectVertexData> objVertData;
			std::vector<ObjectFragmentData> objFragData;
			std::vector<LightData> lightData;
			std::vector<GuiData> guiData;
			std::vector<int> numObjGpuInstances; 
			std::vector<TextureUnitGpuData> meshTextureData, depthmapData, guiTextureData;
			std::string libPath;

			Root();
			void framebuffer_size_callback(GLFWwindow*, int, int);
			void renderMeshes();
			void renderParticles(std::vector<ParticleEmitter*>&);
			void renderGui();
			void initWindow(std::string);
			void initMainFramebuffer(Texture*, Texture*);
			void initBloomFramebuffer();
			void initGuiPlane();
			void updateBloomFramebuffer();
			void updateGuiPlane();
			glm::mat4 calculateProjView(Vector3 = Vector3::VEC_ZERO);
			void updateNodeTree(Shader*, bool);
			void renderMesh(Mesh*, u32&, u32&, bool);

		protected:
			virtual void initMeshRendering(u32 &vao, u32 &vbo, u32 &ebo);
			virtual void initParticleRendering();
			virtual void initGuiRendering();
			virtual void initPostProcessing();
			virtual void postProcess(){}
	};
}

#endif
