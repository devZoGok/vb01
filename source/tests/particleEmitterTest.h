#ifndef PARTICLE_EMIITER_TEST_H
#define PARTICLE_EMIITER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class ParticleEmitter;

	class ParticleEmitterTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(ParticleEmitterTest);
		CPPUNIT_TEST(testHeapSort);
		CPPUNIT_TEST_SUITE_END();

		public:
			ParticleEmitterTest(){}
			~ParticleEmitterTest(){}
			void setUp();
			void tearDown();
		private:
			int numParticles = 100;
			ParticleEmitter *particleEmitter = nullptr;

			void testHeapSort();
	};
}

#endif
