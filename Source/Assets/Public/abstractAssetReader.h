#ifndef ABSTRACT_ASSET_READER_H
#define ABSTRACT_ASSET_READER_H

#include "asset.h"

namespace vb01{
	class AbstractAssetReader{
		public:
			virtual Asset* readAsset(std::string){return nullptr;}
		protected:
			AbstractAssetReader(){}
	};
}

#endif
