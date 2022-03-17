#ifndef MODEL_ASSET_H
#define MODEL_ASSET_H

#include "asset.h"

namespace vb01{
		class Node;

		struct ModelAsset : public Asset{
				Node *rootNode = nullptr;
		};
}

#endif
