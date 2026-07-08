#include "root.h"
#include "node.h"
#include "text.h"
#include "shader.h"
#include "box.h"
#include "quad.h"
#include "camera.h"
#include "light.h"
#include "lineRenderer.h"
#include "assetManager.h"
#include "imageAsset.h"
#include "animationController.h"

#include "stb_image.h"

#include <glad.h>

#include <glfw3.h>

#include <glm.hpp>
#include <ext.hpp>

#include <cstdlib>
#include <iostream>

using namespace std;
using namespace glm;

namespace vb01{
	static Root *root = nullptr;
	Shader *shader = nullptr;

	Root* Root::getSingleton(){
		if(!root)
			root = new Root();

		return root;
	}

	void foo(GLFWwindow *window, int width, int height){
		glViewport(0, 0, width, height);
	}

	void error_callback(int error, const char* msg) {
		std::string s;
		s = " [" + std::to_string(error) + "] " + msg + '\n';
		std::cerr << s << std::endl;
	}

	Root::Root(){
		rootNode = new Node(Vector3::VEC_ZERO);
		guiNode = new Node(Vector3::VEC_ZERO);

		camera = new Camera();
	}

	void Root::start(int width, int height, string libPath, string name){
		this->width = width;
		this->height = height;
		this->libPath = libPath;

		AssetManager::getSingleton()->load(libPath);

		initWindow(name);

		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &NUM_MAX_TEXTURE_ARRAY_SIZE);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &NUM_MAX_TEXTURE_UNITS);

    	glGenBuffers(1, &drawCmdBuffer);
    	glGenBuffers(1, &objVertBuffer);
    	glGenBuffers(1, &objFragBuffer);
    	glGenBuffers(1, &objLightBuffer);

		glGenTextures(1, &skyboxBuffer);

		guiPlaneTextureBuffer = new u32;
		glGenTextures(1, guiPlaneTextureBuffer);

		phongShader = new Shader(libPath + "phong4");
		guiShader = new Shader(libPath + "gui");

		initMeshRendering(meshVAO, meshVBO, meshEBO);
		initMeshRendering(guiVAO, guiVBO, guiEBO);
		initParticleRendering();
		initGuiRendering();
		initPostProcessing();
		initMainFramebuffer(pingPongTextures[0], nullptr);
		initGuiPlane();
	}

	void Root::initMeshRendering(u32 &VAO, u32 &VBO, u32 &EBO){
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);	

		u32 size = sizeof(MeshData::GpuVertex);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, norm)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, uv)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, tan)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, biTan)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, weights)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, boneIndices)));
		glEnableVertexAttribArray(6);

		/*
		for(int i = 0; i < MAX_NUM_SHAPE_KEYS; i++){
			glVertexAttribPointer(7 + i, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, shapeKeyOffsets) + 3 * i * sizeof(float)));
		cout << glGetError() << "\n";
			glEnableVertexAttribArray(7 + i);
		cout << glGetError() << "\n";
		}
		*/
	}

	void Root::initTextureDataOnGpu(int width, int height){
		glBindTexture(GL_TEXTURE_2D, guiPlaneTextureBuffer[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	//TODO check loading of textures with different dimensions 
	void Root::initTextureDataOnGpu(u8 *data, int width, int height, int &bufferId, int &layerId, bool scene){
		int textureUnitId = -1;
		vector<TextureUnitGpuData> &textureData = (scene ? meshTextureData : guiTextureData);

		for(int i = 0; i < textureData.size(); i++)
			if(textureData[i].width == width && textureData[i].height == height){
				textureUnitId = i;
				break;
			}

		if(textureUnitId == -1){
			textureData.push_back(TextureUnitGpuData(width, height));
			textureUnitId = textureData.size() - 1;
			glGenTextures(1, &textureData[textureUnitId].buffer);
		}

		AssetManager *assetManager = AssetManager::getSingleton();
		vector<Asset*> assets = assetManager->getAssets(assetManager->getImageFormats());

		for(int i = 0; i < textureData.size(); i++){
			vector<ImageAsset*> imgAssets;
			TextureUnitGpuData &texData = textureData[i];

			for(Asset *asset : assets){
				ImageAsset *a = (ImageAsset*)asset;

				if(a->loadedToGpu && a->width == texData.width && a->height == texData.height)
					imgAssets.push_back((ImageAsset*)asset);
			}

			glBindTexture(GL_TEXTURE_2D_ARRAY, texData.buffer);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, texData.width, texData.height, imgAssets.size() + 1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			
			for(int j = 0; j < imgAssets.size(); j++)
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, j, imgAssets[j]->width, imgAssets[j]->height, 1, GL_RGB, GL_UNSIGNED_BYTE, imgAssets[j]->image);
			
			if(textureUnitId == i){
				bufferId = i;

				int lid = 0 + (imgAssets.size() == 0 ? 0 : 1);
				layerId = lid;
				texData.numLayers++;

				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, lid, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
			}

			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		}
	}

	void Root::updateRenderNodeData(Node *node){
		vector<Node*> ancestors = node->getAncestors();
		Node *topAncestor = ancestors[ancestors.size() - 1];
		bool scene = (topAncestor == rootNode);

		Vector3 pos = node->getPosition(), scale = node->getScale();
		mat4 model = mat4(1.);

		if(scene){
			Quaternion orient = Quaternion::QUAT_W;

			pos = node->localToGlobalPosition(Vector3::VEC_ZERO);
			orient = node->localToGlobalOrientation(Quaternion::QUAT_W);
			scale = node->getScale();

			Vector3 rotAxis = orient.getAxis();

			if(rotAxis == Vector3::VEC_ZERO)
				rotAxis = Vector3::VEC_I;

			model = translate(model, vec3(pos.x, pos.y, pos.z));
			model = rotate(model, orient.getAngle(), vec3(rotAxis.x, rotAxis.y, rotAxis.z));
			model = glm::scale(model, vec3(scale.x, scale.y, scale.z));

			Vector3 direction = node->getGlobalAxis(2);

			for(Light *light : node->getLights()){
				LightData data;
				Light::Type type = light->getLightType();
				data.type = (int)type;
				data.color[0] = light->getColor().x;
				data.color[1] = light->getColor().y;
				data.color[1] = light->getColor().z;

				switch(type){
					case Light::Type::POINT:
						data.pos[0] = pos.x;
						data.pos[1] = pos.y;
						data.pos[2] = pos.z;
						data.a = light->getAttenuationValues().x;
						data.b = light->getAttenuationValues().y;
						data.c = light->getAttenuationValues().z;
						data.radius = light->getRadius();
						data.useAngle = light->isUseAngle();
						break;
					case Light::Type::DIRECTIONAL:
						//shader->setMat4(proj * view, "lights[" + to_string(thisId) + "].lightMat");
						data.direction[0] = direction.x;
						data.direction[1] = direction.y;
						data.direction[2] = direction.z;
						break;
					case Light::Type::SPOT:
						//shader->setMat4(proj * view, "lights[" + to_string(thisId) + "].lightMat");
						data.pos[0] = pos.x;
						data.pos[1] = pos.y;
						data.pos[2] = pos.z;
						data.direction[0] = direction.x;
						data.direction[1] = direction.y;
						data.direction[2] = direction.z;
						data.a = light->getAttenuationValues().x;
						data.b = light->getAttenuationValues().y;
						data.c = light->getAttenuationValues().z;
						data.innerAngle = light->getInnerAngle();
						data.outerAngle = light->getOuterAngle();
				}
				
				lightData.push_back(data);
			}
		}
		else
			for(Text *text : node->getTexts()){
				Material *mat = text->getMaterial();
				bool texturingEnabled = ((Material::BoolUniform*)mat->getUniform("texturingEnabled"))->value;

				GuiData gui;
				gui.pos[0] = pos.x;
				gui.pos[1] = pos.y;
				gui.pos[2] = pos.z;
				gui.texturingEnabled = texturingEnabled;

				if(gui.texturingEnabled){
					gui.pastTexture[0] = 0;
					gui.pastTexture[1] = 0;
					gui.nextTexture[0] = 0;
					gui.nextTexture[1] = 0;
				}
				else{
					Vector4 diffCol = ((Material::Vector4Uniform*)mat->getUniform("diffuseColor"))->value;
					gui.diffuseColor[0] = diffCol.x;
					gui.diffuseColor[1] = diffCol.y;
					gui.diffuseColor[2] = diffCol.z;
					gui.diffuseColor[3] = diffCol.w;
				}

				guiData.push_back(gui);
			}

		for(Mesh *mesh : node->getMeshes()){
			MeshData &meshData = mesh->getMeshBase();
			Material *mat = mesh->getMaterial();
			bool texturingEnabled = ((Material::BoolUniform*)mat->getUniform("texturingEnabled"))->value;

			if(scene){
				bool lightingEnabled = ((Material::BoolUniform*)mat->getUniform("lightingEnabled"))->value;

				for(int i = 0; i < drawCmdMeshes.size(); i++)
					if(*(drawCmdMeshes[i]) == meshData){
						int sum = 0;

						for(int j = 0; j < i; j++)
							sum += 3 * drawCmdMeshes[j]->numTris;

						DrawElementsIndirectCommand cmd;
						cmd.count = 3 * meshData.numTris;
						cmd.instanceCount = 1;
						cmd.firstIndex = sum;
						cmd.baseVertex = 0;
						cmd.baseInstance = 0;
						drawCmds.push_back(cmd);
					}

				ObjectVertexData ovd;
				ovd.viewProj = projView;
				ovd.model = model;
				//ovd.animated = 0;
				//ovd.bones[0];

				/*
				for(int i = 0; i < meshData.numShapeKeys; i++)
					ovd.shapeKeyFactors[i] = meshData.shapeKeys[i].value;
				*/

				objVertData.push_back(ovd);

				Material *mat = mesh->getMaterial();

				ObjectFragmentData ofd;
				ofd.texturingEnabled = texturingEnabled;

				if(ofd.texturingEnabled){
					ofd.pastTexture[0] = 0;
					ofd.pastTexture[1] = 0;
				}
				else{
					Vector4 diffuseColor = ((Material::Vector4Uniform*)mat->getUniform("diffuseColor"))->value;

					ofd.diffuseColor[0] = diffuseColor.x;
					ofd.diffuseColor[1] = diffuseColor.y;
					ofd.diffuseColor[2] = diffuseColor.z;
					ofd.diffuseColor[3] = diffuseColor.w;
				}

				ofd.lightingEnabled = lightingEnabled;

				if(ofd.lightingEnabled){
					ofd.constLightingEnabled = false;
					ofd.normalMapEnabled = false;
					ofd.castShadow = false;
					ofd.environmentMapEnabled = false;
					ofd.specularMapEnabled = false;

					if(!ofd.specularMapEnabled){
						Vector4 specularColor = ((Material::Vector4Uniform*)mat->getUniform("specularColor"))->value;

						ofd.specularColor[0] = specularColor.x;
						ofd.specularColor[1] = specularColor.y;
						ofd.specularColor[2] = specularColor.z;
						ofd.specularColor[3] = specularColor.w;
						ofd.shinyness;
						ofd.specularStrength;
					}
				}

				objFragData.push_back(ofd);
			}
			else{
				GuiData gui;
				gui.pos[0] = pos.x;
				gui.pos[1] = pos.y;
				gui.pos[2] = pos.z;
				gui.texturingEnabled = texturingEnabled;

				if(gui.texturingEnabled){
					gui.pastTexture[0] = 0;
					gui.pastTexture[1] = 0;
					gui.nextTexture[0] = 0;
					gui.nextTexture[1] = 0;
				}
				else{
					Vector4 diffCol = ((Material::Vector4Uniform*)mat->getUniform("diffuseColor"))->value;
					gui.diffuseColor[0] = diffCol.x;
					gui.diffuseColor[1] = diffCol.y;
					gui.diffuseColor[2] = diffCol.z;
					gui.diffuseColor[3] = diffCol.w;
				}

				guiData.push_back(gui);
			}
		}
	}

	void Root::createCubemap(bool depth, bool fromFile, string paths[6], int mipmapLevel){
		for(int i = 0; i < 6; i++){
			if(!depth){
				if(paths[0] != ""){
					ImageAsset *asset = (ImageAsset*)AssetManager::getSingleton()->getAsset(paths[i]);
					int width = asset->width;

					glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxBuffer);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, width, 0, GL_RGB, GL_UNSIGNED_BYTE, asset->image);	
				}
				else{
					glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, width, 0, GL_RGB, GL_UNSIGNED_INT, NULL);	
				}
			}
			else{
				glBindTexture(GL_TEXTURE_CUBE_MAP, NULL);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);	
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(mipmapLevel > 0){
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		else
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	void Root::initVertexDataOnGpu(MeshData &meshData, u32 &VAO, u32 &VBO, u32 &EBO, bool updateDrawCommands){
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		vector<MeshData::GpuVertex> glVertData = meshData.toGpuVerts();
		u32 vertSize = sizeof(MeshData::GpuVertex), indexSize = sizeof(u32);

		if(updateDrawCommands){
			drawCmdMeshes.push_back(&meshData);
			int currNumIndices = currentIndices.size();

			for(int i = 0; i < 3 * meshData.numTris; i++)
				currentIndices.push_back(meshData.indices[i] + currNumIndices);

			currentGpuVertices.insert(currentGpuVertices.end(), glVertData.begin(), glVertData.end());

			glBufferData(GL_ARRAY_BUFFER, currentGpuVertices.size() * vertSize, currentGpuVertices.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentIndices.size() * indexSize, currentIndices.data(), GL_DYNAMIC_DRAW);
		}
		else{
			u32 numVerts = 3 * meshData.numTris, numIndexes = 3 * meshData.numTris;
			glBufferData(GL_ARRAY_BUFFER, numVerts * vertSize, glVertData.data(), GL_DYNAMIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndexes * indexSize, meshData.indices, GL_DYNAMIC_DRAW);
		}
	}

	void Root::initParticleRendering(){
	}

	void Root::initGuiRendering(){
	}

	void Root::initPostProcessing(){
		//brdfLutPlane = new Quad(Vector3(1, 1, 1) * 2);
		//iblBox = new Box(Vector3::VEC_IJK);

		blurShader = new Shader(libPath + "blur");
		shader = new Shader(Root::getSingleton()->getLibPath() + "line3D");

		for(int i = 0; i < 2; i++)
			pingPongTextures[i] = new Texture(width, height, false);

		//Texture *fragTexture = new Texture(width, height, false);
		//Texture *brightTexture = new Texture(width, height, false);
	}

	void Root::toggleHDR(bool hdr){
		glDeleteFramebuffers((hdr ? 2 : 1), &FBO);
		glDeleteRenderbuffers(1, &RBO);

		Texture *fragTexture = ((Material::TextureUniform*)guiPlane->getMaterial()->getUniform("frag"))->value;
		Texture *brightTexture = ((Material::TextureUniform*)guiPlane->getMaterial()->getUniform("bright"))->value;
		initMainFramebuffer(fragTexture, (hdr ? brightTexture : nullptr));

		this->hdr = hdr;
	}

	void Root::initBloomFramebuffer(){
		glGenFramebuffers(2, pingpongBuffers);

		for(int i = 0; i < 2; i++){
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongBuffers[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(pingPongTextures[i]->getTexture()), 0);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				cout << "Not complete\n";
		}
	}

	void Root::toggleBloom(bool bloom){
		this->bloom = bloom;

		if(bloom)
			initBloomFramebuffer();
		else
			glDeleteFramebuffers(2, pingpongBuffers);
	}

	void Root::initWindow(string name){
		glfwSetErrorCallback(error_callback);

		if (GL_TRUE != glfwInit())
			std::cerr << "Error initialising glfw" << std::endl;

		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

		if(window == NULL){
			cout << "Failed to load window...\n";
			exit(-1);
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, foo);

		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			cout << "Failed to load GLAD.\n";
			exit(-1);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Root::initMainFramebuffer(Texture *fragTexture, Texture *brightTexture){
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		u32 colorAttachments[]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[0], GL_TEXTURE_2D, *guiPlaneTextureBuffer, 0);

		if(brightTexture){
			glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[1], GL_TEXTURE_2D, *brightTexture->getTexture(), 0);
			glDrawBuffers(2, colorAttachments);
		}
		else
			glDrawBuffers(1, colorAttachments);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Not complete\n";

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Root::initGuiPlane(){
		guiPlane = new Quad(Vector3(width, height, -1), false, 0, 0, false);

		initMeshRendering(guiPlaneVAO, guiPlaneVBO, guiPlaneEBO);
		initVertexDataOnGpu(guiPlane->getMeshBase(), guiPlaneVAO, guiPlaneVBO, guiPlaneEBO, false);

		Material *mat = new Material(new Shader(libPath + "postProcess"));
		mat->addTexUniform("frag", pingPongTextures[0], false);
		mat->addTexUniform("bright", pingPongTextures[1], false);
		guiPlane->setMaterial(mat);
	}

	void Root::update(){
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		if(skybox){
			glDepthMask(GL_FALSE);
			glCullFace(GL_FRONT);

			skybox->update();
			skybox->getMaterial()->getShader()->setMat4(calculateProjView(), "projView");
			renderMesh(skybox, skyboxVAO, skyboxBuffer, true);

			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
		}

		AnimationController::getSingleton()->update();

		updateNodeTree(phongShader, false);
		rootNode->update();
		renderMeshes();

		//LineRenderer::getSingleton()->drawLines();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		if(bloom) updateBloomFramebuffer();

		updateGuiPlane();

		glEnable(GL_DEPTH_TEST);
		updateNodeTree(guiShader, true);
		guiNode->update();
		renderGui();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Root::renderMesh(Mesh *mesh, u32 &vao, u32 &texBuffer, bool cubemap){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, texBuffer);

		MeshData meshData = mesh->getMeshBase();
		glBindVertexArray(vao);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, mesh->isWireframe() ? GL_LINE : GL_FILL);
		glDrawElements(GL_TRIANGLES, 3 * meshData.numTris, GL_UNSIGNED_INT, 0);	
	}

	void Root::renderMeshes(){
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, objVertBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ObjectVertexData) * objVertData.size(), objVertData.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, objVertBuffer);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, objFragBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ObjectFragmentData) * objFragData.size(), objFragData.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, objFragBuffer);

		glNamedBufferSubData(objLightBuffer, 0, sizeof(LightData) * lightData.size(), lightData.data());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, objLightBuffer);

		for(int i = 0; i < meshTextureData.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D_ARRAY, meshTextureData[i].buffer);
		}

		glBindVertexArray(meshVAO);

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawCmdBuffer);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * drawCmds.size(), drawCmds.data(), GL_DYNAMIC_DRAW);
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, NULL, drawCmds.size(), 0);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

		glBindVertexArray(0);
	}

	void Root::renderParticles(vector<ParticleEmitter*> &particleEmitters){
	}

	void Root::renderGui(){
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, guiDataBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GuiData) * guiData.size(), guiData.data(), GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, guiDataBuffer);

		for(int i = 0; i < guiTextureData.size(); i++){
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D_ARRAY, guiTextureData[i].buffer);
		}

		glBindVertexArray(guiVAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, guiData.size());	
		glBindVertexArray(0);
	}

	mat4 Root::calculateProjView(Vector3 camPos){
		Vector3 dir = camera->getDirection(), up = camera->getUp();
		mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + dir.x, camPos.y + dir.y, camPos.z + dir.z), vec3(up.x, up.y, up.z));

		float fov = camera->getFov(), nearPlane = camera->getNearPlane(), farPlane = camera->getFarPlane();
		mat4 proj = perspective(radians(fov), (float)width / height, nearPlane, farPlane);

		return proj * view;
	}

	void Root::updateNodeTree(Shader *shader, bool gui){
		shader->use();

		if(!gui){
			objVertData.clear();
			objFragData.clear();
			drawCmds.clear();

			Vector3 camPos = camera->getPosition();
			shader->setVec3(camPos, "camPos");
			projView = calculateProjView(camPos);
		}
		else{
			guiData.clear();
			shader->setVec2(Vector2(width, height), "screen");
		}

		for(int i = 0; i < NUM_MAX_TEXTURE_UNITS; i++)
			shader->setInt(i, "textureSamplers[" + to_string(i) + "]");
	}

	void Root::updateBloomFramebuffer(){
		bool horizontal = false;
		blurShader->use();
		blurShader->setVec2(Vector2(width, height), "screen");

		for(int i = 0; i < blurLevel; i++){
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongBuffers[horizontal]);
			blurShader->setBool(horizontal, "horizontal");

			if(i == 0)
				((Material::TextureUniform*)guiPlane->getMaterial()->getUniform("bright"))->value->select();
			else
				pingPongTextures[!horizontal]->select();

			renderMesh(guiPlane, guiPlaneVAO, meshVAO, false);
			horizontal = !horizontal;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Root::updateGuiPlane(){
		Material *material = guiPlane->getMaterial();
		Shader *shader = material->getShader();

		shader->use();
		shader->setVec2(Vector2(width, height), "screen");
		shader->setBool(bloom, "bloom");
		shader->setBool(hdr, "hdr");
		shader->setFloat(exposure, "exposure");
		shader->setFloat(gamma, "gamma");
		shader->setInt(0, "frag");

		//((Material::TextureUniform*)material->getUniform("frag"))->value->select(0);

		if(hdr){
			shader->setInt(1, "bright");
			((Material::TextureUniform*)material->getUniform("bright"))->value->select(1);
		}

		renderMesh(guiPlane, guiPlaneVAO, guiPlaneTextureBuffer[0], false);
	}

	void Root::createSkybox(string paths[6]){
		Texture *texture = new Texture(paths, 6, true);
		createSkybox(texture);
	}

	void Root::createSkybox(Texture *texture){
		skybox = new Box(Vector3::VEC_IJK * 10, false);

		if(skyboxVBO == -1){
			initMeshRendering(skyboxVAO, skyboxVBO, skyboxEBO);
			initVertexDataOnGpu(skybox->getMeshBase(), skyboxVAO, skyboxVBO, skyboxEBO, false);
		}

		Material *skyboxMat = new Material(new Shader(libPath + "skybox"));
		skyboxMat->addTexUniform("tex", texture, true);
		skybox->setMaterial(skyboxMat);
	}

	void Root::removeSkybox(){
		Material *mat = skybox->getMaterial();
		delete skybox;
		skybox = nullptr;
	}
}
