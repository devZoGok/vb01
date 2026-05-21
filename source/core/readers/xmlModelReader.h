#ifndef XML_MODEL_READER_H
#define XML_MODEL_READER_H

#include "abstractAssetReader.h"
#include "keyframeChannel.h"

#include <vector>
#include <utility>

namespace tinyxml2{
		class XMLElement;
}

namespace vb01{
		class Node;
		class Mesh;
		class Skeleton;
		class Animatable;
		class Animation;
		class Driver;
		struct Asset;

		class XmlModelReader : public AbstractAssetReader{
				public:
						static XmlModelReader* getSingleton();
						Asset* readAsset(std::string);
				private:
						XmlModelReader(){};
						Node* processNode(Node*, tinyxml2::XMLElement*, bool = false);
						Mesh* processMesh(tinyxml2::XMLElement*);
						std::vector<KeyframeChannel> processKeyframeChannells(tinyxml2::XMLElement*);
						void processAnimations(tinyxml2::XMLElement*, std::vector<Animatable*>);
						Skeleton* processSkeleton(Node*, tinyxml2::XMLElement*);
						void setupDrivers(std::vector<Node*>);

						std::vector<std::pair<std::string, Driver*>> driversByNodeNames;
						Node *assetRootNode = nullptr;
		};
}

#endif
