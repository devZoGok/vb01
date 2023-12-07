#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "vector.h"
#include "util.h"
#include "animatable.h"
#include "attachable.h"

#include <glm.hpp>

namespace vb01{
	class Shader;
	class Node;
	class Material;

	class ParticleEmitter : public Animatable, public Attachable{
		public:
			ParticleEmitter(int);
			ParticleEmitter() : Animatable(Animatable::NONE){}
			~ParticleEmitter();
			void update();
			inline Material* getMaterial(){return material;}
			inline void setMaterial(Material *mat){this->material = mat;}
			inline void setNode(Node *node){this->node = node;}
			inline void setSize(Vector2 size){this->size = size;}
			inline void setStartColor(Vector4 color){this->startColor = color;}
			inline void setEndColor(Vector4 color){this->endColor = color;}
			inline void setSpread(float s){this->spread = s;}
			inline void setLowLife(float l){this->lowLife = l;}
			inline void setHighLife(float h){this->highLife = h;}
			inline void setGravity(Vector3 g){this->gravity = g;}
			inline void setSpeed(float s){this->speed = s;}
		private:
			struct Vertex{
				Vector3 pos;
				Vector2 texCoords;
			};
			struct Particle{
				int VAO;
				s64 time = 0, timeToLive = 0;
				Vector3 trans, dir;
				float distToCamPlane;
			};

			void makeHeap(int);
			void updateParticles(Vector3, Vector3);
			void heapSort();
			void setupParticles(Vertex[]);
			void setupDisplay(Vertex[], u32[]);
			void render();
			void animate(float, KeyframeChannel);

			u32 VAO, VBO, EBO, MBO;
			glm::mat4 *matrices;
			int numParticles;
			Particle **particles;
			Material *material = nullptr;
			Vector2 size = Vector2::VEC_IJ;
			Vector3 gravity = Vector3::VEC_ZERO;
			Vector4 startColor = Vector4::VEC_IJKL, endColor = Vector4::VEC_IJKL;
			float speed = .2, spread = 1, lowLife = 1, highLife = 2;

			friend class ParticleEmitterTest;
	};
}

#endif
