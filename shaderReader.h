#ifndef SHADER_READER_H
#define SHADER_READER_H

#include "abstractAssetReader.h"

namespace vb01{
	class ShaderReader : public AbstractAssetReader{
		public:
			static ShaderReader* getSingleton();
			Asset* readAsset(std::string);
		private:
			ShaderReader(){}
	};
}

#endif
