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
			ParticleEmitter(int=1);
			~ParticleEmitter();
			void update();
			inline void setMaterial(Material *mat){this->material=mat;}
			inline void setNode(Node *node){this->node=node;}
			inline void setStartSize(Vector2 size){this->startSize=size;}
			inline void setEndSize(Vector2 size){this->endSize=size;}
			inline void setStartColor(Vector4 color){this->startColor=color;}
			inline void setEndColor(Vector4 color){this->endColor=color;}
		private:
			struct Vertex{
				Vector3 pos;
				Vector2 texCoords;
			};
			struct Particle{
				glm::mat4 mat;
				s64 time=0,timeToLive=0;
				Vector4 color;
				Vector3 dir;
				Vector2 size;
			};

			unsigned int instanceVBO,VAO,VBO,EBO;
			int numParticles;
			Particle *particles;
			Material *material=nullptr;
			Node *node=nullptr;
			Vector2 startSize=Vector2::VEC_IJ,endSize=Vector2::VEC_IJ;
			Vector3 direction=Vector3(0,.1,0);
			Vector4 startColor,endColor;
			float spread=0,lowLife=1,highLife=2;
	};
}

#endif
