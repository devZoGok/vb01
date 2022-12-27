#include "stb_image.h"
#include "root.h"
#include "node.h"
#include "shader.h"
#include "box.h"
#include "quad.h"
#include "lineRenderer.h"
#include "animationController.h"

#include "glad.h"
#include <glfw3.h>
#include <cstdlib>

#include <iostream>

using namespace std;

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

		initWindow(name);

		brdfLutPlane = new Quad(Vector3(1, 1, 1) * 2);
		iblBox = new Box(Vector3::VEC_IJK);

		Texture *fragTexture = new Texture(width, height, false);
		Texture *brightTexture = new Texture(width, height, false);

		initMainFramebuffer(fragTexture, brightTexture);
		initBloomFramebuffer();
		initGuiPlane(fragTexture, brightTexture);

			shader = new Shader(Root::getSingleton()->getLibPath() + "line3D");
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

		for(int i = 0; i < 2; i++)
			glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachments[i], GL_TEXTURE_2D, *((i == 0 ? fragTexture : brightTexture)->getTexture()), 0);

		glDrawBuffers(2, colorAttachments);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Not complete\n";

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Root::initBloomFramebuffer(){
		blurShader = new Shader(libPath + "blur");
		glGenFramebuffers(2, pingpongBuffers);

		for(int i = 0; i < 2; i++){
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongBuffers[i]);
			pingPongTextures[i] = new Texture(width, height, false);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(pingPongTextures[i]->getTexture()), 0);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				cout << "Not complete\n";
		}
	}

	void Root::initGuiPlane(Texture *fragTexture, Texture *brightTexture){
		guiPlane = new Quad(Vector3(width, height, -1), false);
		Material *mat = new Material(libPath + "postProcess");
		mat->addTexUniform("frag", fragTexture, false);
		mat->addTexUniform("bright", brightTexture, false);
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
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
		}

		AnimationController::getSingleton()->update();

		updateNodeTree(rootNode);

		LineRenderer::getSingleton()->drawLines();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		updateBloomFramebuffer();
		updateGuiPlane();

		glEnable(GL_DEPTH_TEST);
		updateNodeTree(guiNode);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//TODO replace sorting algorithm and sort only transparent objects
	void Root::updateNodeTree(Node *node){
			vector<Node*> descendants;
			node->getDescendants(descendants);

			int numDesc = descendants.size();

			for(int i = 0; i < numDesc; i++){
					Vector3 v1 = descendants[i]->getPosition() - camera->getPosition();
					float a1 = v1.norm().getAngleBetween(camera->getDirection());
					float d1 = v1.getLength() * cos(a1);

					for(int j = i; j < numDesc; j++){
							Vector3 v2 = descendants[j]->getPosition() - camera->getPosition();
							float a2 = v2.norm().getAngleBetween(camera->getDirection());
							float d2 = v2.getLength() * cos(a2);

							if(d1 < d2)
									swap(descendants[i], descendants[j]);
					}
			}

			for(Node *desc : descendants)
					desc->update();
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

			guiPlane->render();
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
		shader->setInt(1, "bright");

		((Material::TextureUniform*)material->getUniform("frag"))->value->select(0);
		((Material::TextureUniform*)material->getUniform("bright"))->value->select(1);
		guiPlane->render();
	}

	void Root::createSkybox(string paths[6]){
		skybox = new Box(Vector3(1, 1, 1) * 10);
		Material *skyboxMat = new Material(libPath + "skybox");
		Texture *texture = new Texture(paths, 6, true);
		skyboxMat->addTexUniform("tex", texture, true);
		skybox->setMaterial(skyboxMat);
	}

	void Root::removeSkybox(){
		Material *mat = skybox->getMaterial();
		delete skybox;
		skybox = nullptr;
	}
}
