#include "root.h"
#include "node.h"
#include "box.h"
#include "quad.h"
#include "texture.h"
#include "skeleton.h"
#include "animation.h"

#include "glad.h"
#include <glfw3.h>
#include <glm.hpp>
#include <ext.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace glm;

namespace vb01{
	Mesh::Mesh(MeshData meshBase){
			this->meshBase = meshBase;
			numShapeKeys = meshBase.numShapeKeys;
			shapeKeys = new MeshData::ShapeKey[numShapeKeys];

			for(int i = 0; i < numShapeKeys; i++)
					shapeKeys[i] = meshBase.shapeKeys[i];
	}

	Mesh::~Mesh(){
		for(Mesh *m : meshes)
			delete m;

		if(material)
			delete material;

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
	}

	//TODO allow to init meshes to be drawn statically or dynamically
	void Mesh::construct(){
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);	

		u32 size = sizeof(MeshData::GpuVertex);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		MeshData::GpuVertex *glVertData = meshBase.toGpuVerts();
		glBufferData(GL_ARRAY_BUFFER, 3 * meshBase.numTris * size, glVertData, GL_DYNAMIC_DRAW);
		delete glVertData;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * meshBase.numTris * sizeof(u32), meshBase.indices, GL_STATIC_DRAW);

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

		for(int i = 0; i < meshBase.numShapeKeys; i++){
			glVertexAttribPointer(7 + i, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, shapeKeyOffsets) + 3 * i * sizeof(float)));
			glEnableVertexAttribArray(7 + i);
		}
	}

	void Mesh::initFramebuffer(u32 &framebuffer, u32 &renderbuffer, int width){
		glGenFramebuffers(1, &framebuffer);
		glGenRenderbuffers(1, &renderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, width);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "Mesh framebuffer not complete\n";

		glBindFramebuffer(GL_FRAMEBUFFER, *Root::getSingleton()->getFBO());
	}

	void Mesh::updateVerts(MeshData meshData){
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		MeshData::GpuVertex *glVertData = meshBase.toGpuVerts();
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(MeshData::GpuVertex) * meshBase.numTris, glVertData);
		delete glVertData;
	}

	void Mesh::update(){
		Root *root = Root::getSingleton();
		Camera *cam = root->getCamera();
		Vector3 camPos = Vector3::VEC_ZERO, pos = Vector3::VEC_ZERO, scale = Vector3::VEC_IJK;
		Quaternion orient = Quaternion::QUAT_W;

		if(node){
			pos = node->localToGlobalPosition(Vector3::VEC_ZERO);
			orient = node->localToGlobalOrientation(Quaternion::QUAT_W);
			scale = node->getScale();
			camPos = cam->getPosition();
		}

		Vector3 rotAxis = orient.getAxis();

		if(rotAxis == Vector3::VEC_ZERO)
			rotAxis = Vector3::VEC_I;

		mat4 model = mat4(1.);
		model = translate(model, vec3(pos.x, pos.y, pos.z));
		model = rotate(model, orient.getAngle(), vec3(rotAxis.x, rotAxis.y, rotAxis.z));
		model = glm::scale(model, vec3(scale.x, scale.y, scale.z));

		Vector3 dir = cam->getDirection(), up = cam->getUp();
		mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + dir.x, camPos.y + dir.y, camPos.z + dir.z), vec3(up.x, up.y, up.z));

		float fov = cam->getFov(), width = root->getWidth(), height = root->getHeight(), nearPlane = cam->getNearPlane(), farPlane = cam->getFarPlane();
		mat4 proj = perspective(radians(fov), width / height, nearPlane, farPlane);

		material->update();
		Shader *shader = material->getShader();

		if(skeleton)
			updateSkeleton(shader);

		if(meshBase.numShapeKeys > 0)
			updateShapeKeys(shader);

		if(reflect)
			updateReflection(pos);

		shader->setBool(castShadow, "castShadow");
		shader->setBool(skeleton, "animated");
		shader->setMat4(view, "view");
		shader->setMat4(proj, "proj");
		shader->setVec3(camPos, "camPos");
		shader->setMat4(model, "model");

		if(shader->getName() == "gui"){
			shader->setVec2(Vector2((float)width, (float)height), "screen");

			if(node)
				shader->setVec3(pos, "pos");
		}

		render();
	}

	void Mesh::updateShapeKeys(Shader *shader){
		shader->editShader(Shader::VERTEX_SHADER, 5, "const int numShapeKeys=" + to_string(numShapeKeys) + ";");

		for(int i = 0; i < numShapeKeys; i++){
			if(shapeKeys[i].value < shapeKeys[i].minValue)
				shapeKeys[i].value = shapeKeys[i].minValue;
			else if(shapeKeys[i].value > shapeKeys[i].maxValue)
				shapeKeys[i].value = shapeKeys[i].maxValue;

			shader->setFloat(shapeKeys[i].value, "shapeKeyFactors[" + to_string(i) + "]");
		}
	}

	void Mesh::updateSkeleton(Shader *shader){
		skeleton->update();
		shader->editShader(Shader::VERTEX_SHADER, 2, "const int numBones=" + to_string(skeleton->getNumBones()) + ";");
		shader->editShader(Shader::VERTEX_SHADER, 3, "const int numVertGroups=" + to_string(meshBase.numVertexGroups) + ";");
		Node *modelNode = skeleton->getRootBone()->getParent();

		for(int i = 0; i < skeleton->getNumBones(); i++){
			Bone *bone = skeleton->getBone(i), *parent = (Bone*)bone->getParent();
			Vector3 boneAxis[]{
				bone->getInitAxis(0),
				bone->getInitAxis(1),
				bone->getInitAxis(2)
			};

			int vertGroupId = -1;

			for(int j = 0; j < meshBase.numVertexGroups; j++)
				if(bone->getName() == meshBase.vertexGroups[j])
					vertGroupId = j;

			int parentId = -1;

			for(int j = 0; j < skeleton->getNumBones(); j++)
				if(skeleton->getBone(j) == parent){
					parentId = j;
					break;
				}	

			Vector3 posePos = bone->getPosePos();
			Quaternion poseRot = bone->getPoseRot();
			Vector3 rotAxis = poseRot.getAxis(); 
			rotAxis = (boneAxis[0] * rotAxis.x + boneAxis[1] * rotAxis.y + boneAxis[2] * rotAxis.z).norm();
			Vector3 trans = boneAxis[0] * posePos.x + boneAxis[1] * posePos.y + boneAxis[2] * posePos.z;
			Vector3 scale = bone->getPoseScale();
			Vector3 bonePos = bone->getBoneSpaceRestPos(skeleton->getRootBone());
			
			shader->setVec3(bonePos, "bones[" + to_string(i) + "].pos");
			shader->setVec3(trans, "bones[" + to_string(i) + "].trans");
			shader->setVec3(rotAxis, "bones[" + to_string(i) + "].rotAxis");
			shader->setFloat(poseRot.getAngle(), "bones[" + to_string(i) + "].angle");
			shader->setVec3(scale, "bones[" + to_string(i) + "].scale");
			shader->setInt(vertGroupId, "bones[" + to_string(i) + "].vertGroup");
			shader->setInt(parentId, "bones[" + to_string(i) + "].parent");
		}
	}

	void Mesh::updatePrefilterMap(Vector3 pos, mat4 &proj, vec3 dirs[6]){
		/*
		Node *rootNode = root->getRootNode();
		vector<Node*> descendants;
		rootNode->getDescendants(descendants);

		for(Node *node : descendants){
			for(Mesh *mesh : node->getMeshes())
				if(mesh != this && mesh->isReflectible()){
					Vector3 position = node->localToGlobalPosition(Vector3::VEC_ZERO);
					Quaternion rotation = node->localToGlobalOrientation(Quaternion::QUAT_W);
					float far = 100;

					vec3 p = vec3(position.x, position.y, position.z);
					mat4 proj = perspective(radians(90.f), 1.f, .1f, far);
					mat4 model = translate(mat4(1.), p);
					Vector3 rotAxis = rotation.norm().getAxis();

					if(rotAxis == Vector3::VEC_ZERO)
						rotAxis = Vector3::VEC_I;

					model = rotate(model, rotation.norm().getAngle(), vec3(rotAxis.x, rotAxis.y, rotAxis.z));

					environmentShader->setMat4(model, "model");
					environmentShader->setBool(true, "point");
					environmentShader->setVec3(pos, "lightPos");
					environmentShader->setFloat(far, "farPlane");
					}

					mesh->render();
				}
		}
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, preFilterFramebuffer);

		glViewport(0, 0, preFilterMapSize, preFilterMapSize);

		Root *root = Root::getSingleton();
		Mesh *skybox = root->getSkybox();
		Material *mat = skybox->getMaterial();
		mat->update();

		Shader *shader = mat->getShader();
		shader->setMat4(proj, "proj");

		for(int i = 0; i < 6; i++){
			shader->setMat4(lookAt(vec3(0.0), dirs[i], getUpVec(dirs[i])), "view");

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *prefilterMap->getTexture(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if(skybox) {
				glDepthMask(GL_FALSE);
				glCullFace(GL_FRONT);
				skybox->render();
				glDepthMask(GL_TRUE);
				glCullFace(GL_BACK);
			}
		}

		glBindRenderbuffer(GL_RENDERBUFFER, *root->getRBO());
		glBindFramebuffer(GL_FRAMEBUFFER, *root->getFBO());
		glViewport(0, 0, root->getWidth(), root->getHeight());
	}

	void Mesh::updateIrradianceMap(Vector3 pos, mat4 &proj, vec3 dirs[6]){
		glBindFramebuffer(GL_FRAMEBUFFER, irrandianceFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, irradianceRenderbuffer);

		int width = irradianceMapSize;
		glViewport(0, 0, width, width);
		irradianceShader->use();
		irradianceShader->setMat4(proj, "proj");
		irradianceShader->setInt(0, "environmentMap");
		irradianceShader->setFloat(irradianceSampleDelta, "sampleDelta");
		prefilterMap->select();

		Root *root = Root::getSingleton();

		for(int i = 0; i < 6; i++){
			irradianceShader->setMat4(lookAt(vec3(0.0), dirs[i], getUpVec(dirs[i])), "view");

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *irradianceMap->getTexture(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDepthMask(GL_FALSE);
			glCullFace(GL_FRONT);
			root->getIblBox()->render();
			glDepthMask(GL_TRUE);
			glCullFace(GL_BACK);
		}

		glViewport(0, 0, root->getWidth(), root->getHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, *root->getRBO());
		glBindFramebuffer(GL_FRAMEBUFFER, *root->getFBO());
	}

	void Mesh::updatePostfilterMap(mat4 &proj, vec3 dirs[6]){
		glBindFramebuffer(GL_FRAMEBUFFER, postFilterFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, postFilterRenderbuffer);
		
		environmentShader->use();
		environmentShader->setMat4(proj, "proj");
		environmentShader->setInt(0, "environmentMap");
		environmentShader->setFloat(preFilterMapSize, "resolution");
		environmentShader->setUnsignedInt(numSamples, "numSamples");

		prefilterMap->select();

		Root *root = Root::getSingleton();
		int numMipmaps = postfilterMap->getMipmapLevel();

		for(int i = 0; i < numMipmaps; ++i){
			u32 width = environmentMapSize * pow(0.5, i);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, width);
			glViewport(0, 0, width, width);

			environmentShader->setFloat((float)i / numMipmaps, "roughness");

			for(int j = 0; j < 6; j++){
				environmentShader->setMat4(lookAt(vec3(0.0), dirs[j], getUpVec(dirs[j])), "view");

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, *postfilterMap->getTexture(), i);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glDepthMask(GL_FALSE);
				glCullFace(GL_FRONT);
				root->getIblBox()->render();
				glDepthMask(GL_TRUE);
				glCullFace(GL_BACK);
			}
		}

		glBindRenderbuffer(GL_RENDERBUFFER, *root->getRBO());
		glBindFramebuffer(GL_FRAMEBUFFER, *root->getFBO());
		glViewport(0, 0, root->getWidth(), root->getHeight());
	}

	void Mesh::updateBrdfLut(){
		glBindFramebuffer(GL_FRAMEBUFFER, brdfFramebuffer);

		int width = brdfMapSize;
		glViewport(0, 0, width, width);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBindRenderbuffer(GL_RENDERBUFFER, brdfRenderbuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *brdfIntegrationMap->getTexture(), 0);

		brdfIntegrationShader->use();
		brdfIntegrationShader->setUnsignedInt(numSamples, "numSamples");

		Root *root = Root::getSingleton();
		root->getBrdfLutPlane()->render();

		glBindRenderbuffer(GL_RENDERBUFFER, *root->getRBO());
		glBindFramebuffer(GL_FRAMEBUFFER, *root->getFBO());
		glViewport(0, 0, root->getWidth(), root->getHeight());
	}

	void Mesh::updateReflection(Vector3 pos){
		vec3 dirs[]{ vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0), vec3(0, 0, 1), vec3(0, 0, -1) };
		mat4 proj = perspective(radians(90.f), 1.0f, 0.1f, 100.0f);

		updatePrefilterMap(pos, proj, dirs);
		updateIrradianceMap(pos, proj, dirs);
		updatePostfilterMap(proj, dirs);
		updateBrdfLut();

		Shader *shader = material->getShader();
		shader->use();

		int irradianceId = 10, brdfId = 11, postFilterId = 12;

		shader->setInt(irradianceId, "irradianceMap");
		shader->setInt(brdfId, "brdfIntegrationMap");
		shader->setInt(postFilterId, "postFilterMap");

		irradianceMap->select(irradianceId);
		brdfIntegrationMap->select(brdfId);
		postfilterMap->select(postFilterId);
	}

	void Mesh::render(){
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		glDrawElements(GL_TRIANGLES, 3 * meshBase.numTris, GL_UNSIGNED_INT, 0);	
	}

	void Mesh::setReflect(bool reflect){
		this->reflect = reflect;

		if(reflect && !environmentShader){
			string libPath = Root::getSingleton()->getLibPath();
			environmentShader = new Shader(libPath + "environmentPreFilter");
			irradianceShader = new Shader(libPath + "irradiance");
			brdfIntegrationShader = new Shader(libPath + "brdfIntegration");

			prefilterMap = new Texture(preFilterMapSize, false);
			irradianceMap = new Texture(irradianceMapSize, false);
			postfilterMap = new Texture(environmentMapSize, false, 5);
			brdfIntegrationMap = new Texture(brdfMapSize, brdfMapSize, false);

			initFramebuffer(preFilterFramebuffer, preFilterRenderbuffer, preFilterMapSize);
			initFramebuffer(irrandianceFramebuffer, irradianceRenderbuffer, irradianceMapSize);
			initFramebuffer(postFilterFramebuffer, postFilterRenderbuffer, environmentMapSize);
			initFramebuffer(brdfFramebuffer, brdfRenderbuffer, brdfMapSize);
		}
	}
}
