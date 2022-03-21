#include "particleEmitterTest.h"
#include "particleEmitter.h"

#include <algorithm>

using namespace std;

namespace vb01{
	void ParticleEmitterTest::setUp(){
		particleEmitter = new ParticleEmitter();
		particleEmitter->numParticles = numParticles;
		particleEmitter->particles = new ParticleEmitter::Particle*[numParticles];

		for(int i = 0; i < numParticles; i++){
			ParticleEmitter::Particle *particle = new ParticleEmitter::Particle;
			particle->distToCamPlane = rand() % 1000;
			particleEmitter->particles[i] = particle;
		}
	}

	void ParticleEmitterTest::tearDown(){
	}

	void ParticleEmitterTest::testHeapSort(){
		particleEmitter->heapSort();

		for(int i = 1; i < numParticles; i++){
			ParticleEmitter::Particle *prevPart = particleEmitter->particles[i - 1];
			ParticleEmitter::Particle *currPart = particleEmitter->particles[i];
			CPPUNIT_ASSERT(prevPart->distToCamPlane >= currPart->distToCamPlane);
		}
	}
}
