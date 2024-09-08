#include "shaderReader.h"
#include "shaderAsset.h"

#include <sstream>
#include <fstream>

namespace vb01{
	using namespace std;

	static ShaderReader *shaderReader = nullptr;

	ShaderReader* ShaderReader::getSingleton(){
		if(!shaderReader)
			shaderReader = new ShaderReader();
		
		return shaderReader;
	}

	Asset* ShaderReader::readAsset(string path){
		ifstream shaderFile;
		shaderFile.open(path);
		
		stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		
		shaderFile.close();

		return new ShaderAsset(path, shaderStream.str());
	}
}
