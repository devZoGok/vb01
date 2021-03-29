#include "model.h"
#include "light.h"
#include "node.h"
#include "root.h"
#include "shader.h"
#include "material.h"
#include "mesh.h"

#include "glad.h"
#include <glfw3.h>
#include <ext.hpp>
#include <iostream>

using namespace glm;
using namespace std;

namespace vb01{
	Light::Light(Type t){
		this->type = t;

		initDepthMap();
	}

	Light::~Light(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &depthmapFBO);

		delete depthMap;

		delete depthMapShader;
	}

	void Light::animate(float value, KeyframeChannel keyframeChannel){
		switch(keyframeChannel.type){
			case KeyframeChannel::DIFFUSE_COLOR_X:
				color.x = value;
				break;
			case KeyframeChannel::DIFFUSE_COLOR_Y:
				color.y = value;
				break;
			case KeyframeChannel::DIFFUSE_COLOR_Z:
				color.z = value;
				break;
			case KeyframeChannel::SPOTLIGHT_INNER_ANGLE:
				innerAngle = value;
				break;
			case KeyframeChannel::SPOTLIGHT_OUTER_ANGLE:
				outerAngle = value;
				break;
		}
	}

	void Light::initDepthMap(){
		string basePath = "../../vb01/depthMap.";

		glGenFramebuffers(1, &depthmapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
		if(type == POINT){
			depthMap = new Texture(depthMapSize);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *(depthMap->getTexture()), 0);
			depthMapShader = new Shader(basePath + "vert", basePath + "frag", basePath + "geo");
		}
		else{
			depthMap = new Texture(depthMapSize, depthMapSize, true);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *(depthMap->getTexture()), 0);
			depthMapShader = new Shader(basePath + "vert", basePath + "frag");
		}
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Not complete\n";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Light::update(){
		Root *root = Root::getSingleton();
		Node *rootNode = root->getRootNode();

		vector<Node*> descendants;
		rootNode->getDescendants(descendants);
		descendants.push_back(rootNode);
		vector<Light*> lights;
		vector<Material*> materials;

		for(Node *d : descendants){
			for(Light *l : d->getLights())
				lights.push_back(l);
			for(Mesh *m : d->getMeshes())
				materials.push_back(m->getMaterial());
		}

		int thisId = -1;
		for(int i = 0; i < lights.size(); i++)
			if(lights[i] == this)
				thisId = i;

		mat4 proj = mat4(1.), view = mat4(1.);
		renderShadow(descendants, proj, view);
		updateShader(materials, thisId, proj, view);

	}

	void Light::renderShadow(std::vector<Node*> descendants, mat4 &proj, mat4 &view){
		Root *root = Root::getSingleton();
		Vector3 position = node->getPosition(), direction = node->getGlobalAxis(2);

		glViewport(0, 0, depthMapSize, depthMapSize);
		glBindFramebuffer(GL_FRAMEBUFFER, depthmapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		for(Node *n : descendants){
			vector<Mesh*> meshes = n->getMeshes();
			for(Mesh *mesh : meshes){
				Material *mat = mesh->getMaterial();
				if(mesh->isCastShadow() && n->isVisible() && mat && mat->isLightingEnabled()){
					Vector3 pos = n->localToGlobalPosition(Vector3::VEC_ZERO);
					Quaternion rot = n->localToGlobalOrientation(Quaternion::QUAT_W);
					Vector3 rotAxis = rot.norm().getAxis();

					if(rotAxis == Vector3::VEC_ZERO)
						rotAxis = Vector3::VEC_I;
					mat4 model = translate(mat4(1.f), vec3(pos.x, pos.y, pos.z));
					model = rotate(model, rot.getAngle(), vec3(rotAxis.x, rotAxis.y, rotAxis.z));

					vec3 lightPos, dir = vec3(direction.x, direction.y, direction.z);
					if(type == DIRECTIONAL){
						lightPos = vec3(pos.x, pos.y, pos.z) - .5f * farPlane * dir;
						float orthoCorner = 10.f;
						proj = ortho(-orthoCorner, orthoCorner, -orthoCorner, orthoCorner, nearPlane, farPlane);
					}
					else{
						lightPos = vec3(position.x, position.y, position.z);
						proj = perspective(radians(90.f), 1.f, nearPlane, farPlane);
					}
					
					Vector3 upDir = direction.cross(Vector3(0, 1, 0));
					if(upDir == Vector3::VEC_ZERO)
						upDir = Vector3::VEC_I;
					vec3 up = vec3(upDir.x, upDir.y, upDir.z);
					view = lookAt(lightPos, lightPos + dir, up);

					depthMapShader->use();
					depthMapShader->setBool(type == POINT, "point");
					depthMapShader->setMat4(model, "model");
					depthMapShader->setFloat(farPlane, "farPlane");
					depthMapShader->setVec3(position, "lightPos");
					if(type == POINT){
						vec3 dirs[] = {vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1)};
						for(int i = 0; i < 6; i++){
							vec3 upVec;
							if(1 < i && i < 4)
								upVec = vec3(0, 0, -1);
							else
								upVec = vec3(0, -1, 0);
							depthMapShader->setMat4(proj * lookAt(lightPos, lightPos + dirs[i], upVec), "shadowMat[" + to_string(i) + "]");
						}
					}
					else
						depthMapShader->setMat4(proj * view, "lightMat");

					mesh->render();
				}
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, *(root->getFBO()));
		glViewport(0, 0, root->getWidth(), root->getHeight());
	}

	void Light::updateShader(vector<Material*> materials, int thisId, mat4 &proj, mat4 &view){
		Vector3 position = node->localToGlobalPosition(Vector3::VEC_ZERO), direction = node->getGlobalAxis(2);
		for(Material *m : materials){
			Shader *shader = m->getShader();
			shader->use();
			shader->setInt((int)type, "light[" + to_string(thisId) + "].type");
			shader->setVec3(color, "light[" + to_string(thisId) + "].color");
			shader->setFloat(nearPlane, "light[" + to_string(thisId) + "].near");
			shader->setFloat(farPlane, "light[" + to_string(thisId) + "].far");
			shader->setInt(12, "light[" + to_string(thisId) + "].depthMapCube");
			shader->setInt(13, "light[" + to_string(thisId) + "].depthMap");
			depthMap->select(type == POINT ? 12 : 13);

			switch(type){
				case POINT:
					shader->setVec3(position, "light[" + to_string(thisId) + "].pos");
					shader->setFloat(attenuationValues.x, "light[" + to_string(thisId) + "].a");
					shader->setFloat(attenuationValues.y, "light[" + to_string(thisId) + "].b");
					shader->setFloat(attenuationValues.z, "light[" + to_string(thisId) + "].c");
					break;
				case DIRECTIONAL:
					shader->setMat4(proj * view, "light[" + to_string(thisId) + "].lightMat");
					shader->setVec3(direction, "light[" + to_string(thisId) + "].direction");
					break;
				case SPOT:
					shader->setMat4(proj * view, "light[" + to_string(thisId) + "].lightMat");
					shader->setVec3(position, "light[" + to_string(thisId) + "].pos");
					shader->setVec3(direction,"light[" + to_string(thisId) + "].direction");
					shader->setFloat(attenuationValues.x, "light["+to_string(thisId) + "].a");
					shader->setFloat(attenuationValues.y, "light["+to_string(thisId) + "].b");
					shader->setFloat(attenuationValues.z, "light["+to_string(thisId) + "].c");
					shader->setFloat(innerAngle, "light[" + to_string(thisId) + "].innerAngle");
					shader->setFloat(outerAngle, "light[" + to_string(thisId) + "].outerAngle");
					break;
			}
		}
	}
}
