#include "shaderTest.h"
#include "shader.h"

#include <string>

using namespace std;

namespace vb01{
	void ShaderTest::setUp(){
		shader = new Shader();
		shader->vString = "var x = 1\nvar y = 1\nvar z = 1";
	}

	void ShaderTest::tearDown(){
	}

	void ShaderTest::testReplaceLine(){
		string replacement = "var y = 10";
		shader->replaceLine(Shader::ShaderType::VERTEX_SHADER, 1, replacement);
		size_t spot = shader->vString.find(replacement);
		CPPUNIT_ASSERT(spot != string::npos);
	}
}
