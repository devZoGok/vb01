#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include"vector.h"
#include"util.h"
#include<glm.hpp>

namespace vb01{
	class Shader;
	class Node;
	class Material;

	class ParticleEmitter{
		public:
			ParticleEmitter(int = 1);
			~ParticleEmitter();
			void update();
			inline void setMaterial(Material *mat){this->material = mat;}
			inline void setNode(Node *node){this->node = node;}
			inline void setStartSize(Vector2 size){this->startSize = size;}
			inline void setEndSize(Vector2 size){this->endSize = size;}
			inline void setStartColor(Vector4 color){this->startColor = color;}
			inline void setEndColor(Vector4 color){this->endColor = color;}
			inline void setSpread(float s){this->spread = s;}
			inline void setLowLife(float l){this->lowLife = l;}
			inline void setHighLife(float h){this->highLife = h;}
			inline void setGravity(Vector3 g){this->gravity = g;}
			inline Node* getNode(){return node;}
			inline Vector3 getDirection(){return direction;}
			void setDirection(Vector3 dir);
		private:
			struct Vertex{
				Vector3 pos;
				Vector2 texCoords;
			};
			struct Particle{
				int VAO;
				s64 time = 0, timeToLive = 0;
				Vector4 color;
				Vector3 dir, trans;
				Vector2 size;
				float d;
			};

			void makeHeap(int);
			void updateParticles(Vector3, Vector3);
			void heapSort();
			void setupParticles(Vertex[]);
			void setupDisplay(Vertex[], u32[]);
			void render();

			unsigned int VAO, VBO, EBO, MBO;
			glm::mat4 *matrices;
			int numParticles;
			Particle **particles;
			Material *material = nullptr;
			Node *node = nullptr;
			Vector2 startSize = Vector2::VEC_IJ, endSize = Vector2::VEC_IJ;
			Vector3 direction = Vector3(0, .1, 0), gravity = Vector3::VEC_ZERO;
			Vector4 startColor, endColor;
			float spread = 1, lowLife = 1, highLife = 2;
	};
}

#endif
