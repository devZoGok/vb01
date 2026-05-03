#ifndef SHADER_TEST_H
#define SHADER_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace vb01{
	class Shader;

	class ShaderTest : public CppUnit::TestFixture{
		CPPUNIT_TEST_SUITE(ShaderTest);
		CPPUNIT_TEST(testReplaceLine);
		CPPUNIT_TEST_SUITE_END();

		public:
			ShaderTest(){}
			~ShaderTest(){}
			void setUp();
			void tearDown();
		private:
			Shader *shader = nullptr;

			void testReplaceLine();
	};
}

#endif
