#include <algorithm>
#include <glad.h>
#include <glfw3.h>
#include <cstdlib>
#include <ext.hpp>

#include "particleEmitter.h"
#include "shader.h"
#include "material.h"
#include "node.h"
#include "root.h"
#include "camera.h"

using namespace glm;
using namespace std;

namespace vb01{
	ParticleEmitter::ParticleEmitter(int numParticles){
		this->numParticles = numParticles;

		Vertex v1, v2, v3, v4, v5, v6;
		ParticleEmitter::Vertex vertices[] = {v1, v2, v3, v4, v5, v6};
		u32 indices[] = {0, 1, 2, 3, 4, 5};

		setupParticles(vertices);
		setupDisplay(vertices, indices);
	}

	void ParticleEmitter::setupParticles(Vertex vertices[]){
		particles = new Particle*[numParticles];

		Vector2 size = Vector2(.5,.5);
		vertices[0].pos = Vector3(size.x / 2, size.y / 2, 0);
		vertices[0].texCoords = Vector2(1, 1);

		vertices[1].pos = Vector3(-size.x / 2, size.y / 2, 0);
		vertices[1].texCoords = Vector2(0, 1);

		vertices[2].pos = Vector3(-size.x / 2,-size.y / 2, 0);
		vertices[2].texCoords=Vector2(0,0);

		vertices[3].pos = Vector3(-size.x / 2, -size.y / 2, 0);
		vertices[3].texCoords = Vector2(0, 0);

		vertices[4].pos = Vector3(size.x / 2, -size.y / 2, 0);
		vertices[4].texCoords = Vector2(1, 0);

		vertices[5].pos = Vector3(size.x / 2, size.y / 2, 0);
		vertices[5].texCoords = Vector2(1, 1);
		
		matrices = new mat4[numParticles];

		for(int i = 0; i < numParticles; i++){
			Particle *p = new Particle;
			particles[i] = p;
			matrices[i] = mat4(1.f);
		}

	}

	void ParticleEmitter::setupDisplay(Vertex vertices[], u32 indices[]){
		u32 MBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &MBO);
		glBindBuffer(GL_ARRAY_BUFFER, MBO);
		glBufferData(GL_ARRAY_BUFFER, numParticles * sizeof(mat4), &matrices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(1 * sizeof(vec4)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(2 * sizeof(vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(3 * sizeof(vec4)));
		glEnableVertexAttribArray(5);

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
	}

	ParticleEmitter::~ParticleEmitter(){
		glDeleteVertexArrays(1, &VAO);	
		glDeleteBuffers(1, &VBO);	

		delete material;

		delete[] particles;
	}

	void ParticleEmitter::makeHeap(int offset){
		bool heap = false;
		while(!heap){
			bool end = true;
			for(int i = 0; 2 * i + 1 + offset < numParticles; i++){
				if(2 * i + 2 + offset < numParticles && (particles[i + offset]->distToCamPlane < particles[2 * i + 1 + offset]->distToCamPlane ||
						   	particles[i + offset]->distToCamPlane < particles[2 * i + 2 + offset]->distToCamPlane))
				{
					bool leftChild = particles[2 * i + 1 + offset]->distToCamPlane > particles[2 * i + 2 + offset]->distToCamPlane;
					swap(particles[i + offset], leftChild ? particles[2 * i + 1 + offset] : particles[2 * i + 2 + offset]);
				}
				else if(2 * i + 2 + offset == numParticles && particles[i + offset]->distToCamPlane < particles[2 * i + 1 + offset]->distToCamPlane)
					swap(particles[i + offset], particles[2 * i + 1 + offset]);
			}
			for(int i = 0; 2 * i + 1 < numParticles; i++){
				if(2 * i + 2 + offset < numParticles && (particles[i + offset]->distToCamPlane < particles[2 * i + 1 + offset]->distToCamPlane ||
						   	particles[i + offset]->distToCamPlane < particles[2 * i + 2 + offset]->distToCamPlane))
				{
					end = false;
				}
				else if(2 * i + 2 + offset == numParticles && particles[i + offset]->distToCamPlane < particles[2 * i + 1 + offset]->distToCamPlane)
					end = false;
			}
			if(end)
				heap = true;
		}
	}
	
	void ParticleEmitter::heapSort(){
		for(int i = 0; i < numParticles; i++)
			makeHeap(i);
	}
	
	void ParticleEmitter::update(){
		Root *root = Root::getSingleton();
		Camera *cam = root->getCamera();
		float fov = cam->getFov(), width = root->getWidth(), height = root->getHeight();
		float nearPlane = cam->getNearPlane(), farPlane = cam->getFarPlane();
		Vector3 pos = node->getPosition();
		Vector3 camDir = cam->getDirection(), up = cam->getUp(), left = cam->getLeft(), camPos = cam->getPosition();

		mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + camDir.x, camPos.y + camDir.y, camPos.z + camDir.z), vec3(up.x, up.y, up.z));
		mat4 proj = perspective(radians(fov), width / height, nearPlane, farPlane);

		material->update();	

		Shader *shader = material->getShader();
		const string line = "const int numParticles = " + to_string(numParticles) + ";";
		shader->editShader(Shader::VERTEX_SHADER, 10, line);
		shader->editShader(Shader::FRAGMENT_SHADER, 6, line);

		updateParticles(camDir, camPos);

		shader->setMat4(view, "view");
		shader->setMat4(proj, "proj");
		shader->setVec4(startColor, "startColor");
		shader->setVec4(endColor, "endColor");
		shader->setVec2(startSize, "startSize");
		shader->setVec2(endSize, "endSize");

		heapSort();
		render();
	}

	void ParticleEmitter::updateParticles(Vector3 camDir, Vector3 camPos){
		Shader *shader = material->getShader();
		Vector3 nodePos = node->localToGlobalPosition(Vector3::VEC_ZERO);
		Vector3 nodeDir = node->getGlobalAxis(2);
		Vector3 nodeUp = node->getGlobalAxis(1);
		Vector3 nodeLeft = node->getGlobalAxis(0);

		for(int i = 0; i < numParticles; i++){
			Vector3 dir = nodeDir;
			if(getTime() - particles[i]->time >= particles[i]->timeToLive){
				float factor = (float)(rand() % 100) / 100;
				particles[i]->time = getTime();
				particles[i]->timeToLive = s64(1000 * ((highLife - lowLife) * factor + lowLife));

				float spr1 = float(rand() % (int)spread) / 180 * PI, spr2 = float(rand() % 360) / 180 * PI;
				Vector3 ra1 = (nodeUp.cross(dir)).norm(), ra2 = dir.norm();
				if(ra1 == Vector3::VEC_ZERO)
				   	ra1 = Vector3::VEC_I;
				dir = Quaternion(spr1, ra1) * dir;
				dir = Quaternion(spr2, ra2) * dir;

				particles[i]->trans = nodePos;
			}

			dir = dir.norm() * speed + gravity;
			particles[i]->trans = particles[i]->trans + dir;
			particles[i]->distToCamPlane = cos(camDir.getAngleBetween((particles[i]->trans - camPos).norm())) * camPos.getDistanceFrom(particles[i]->trans);

			float lifePercentage = (float)(getTime() - particles[i]->time) / particles[i]->timeToLive;

			shader->setFloat(lifePercentage, "lifePercentage[" + to_string(i) + "]");
			shader->setVec3(particles[i]->trans, "trans[" + to_string(i) + "]");
		}
	}

	void ParticleEmitter::render(){
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, numParticles);	
	}

	void ParticleEmitter::animate(float value, KeyframeChannel keyframeChannel){
		switch(keyframeChannel.type){
			case KeyframeChannel::PARTICLE_EMITTER_SPEED:
				speed = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_SPREAD:
				spread = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_START_COLOR_W:
				startColor.w = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_START_COLOR_X:
				startColor.x = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_START_COLOR_Y:
				startColor.y = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_START_COLOR_Z:
				startColor.z = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_END_COLOR_W:
				endColor.w = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_END_COLOR_X:
				endColor.x = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_END_COLOR_Y:
				endColor.y = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_END_COLOR_Z:
				endColor.z = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_START_SIZE_X:
				startSize.x = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_START_SIZE_Y:
				startSize.y = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_END_SIZE_X:
				endSize.x = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_END_SIZE_Y:
				endSize.y = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_LOW_LIFE:
				lowLife = value;
				break;
			case KeyframeChannel::PARTICLE_EMITTER_HIGH_LIFE:
				highLife = value;
				break;
		}
	}
}
