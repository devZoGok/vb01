#ifndef SHADER_ASSET_H
#define SHADER_ASSET_H

#include "asset.h" 

namespace vb01{
	struct ShaderAsset : public Asset{
		std::string shaderString = ""; 

		ShaderAsset(std::string p, std::string str){
			path = p;
			shaderString = str;
		}
	};
}

#endif
