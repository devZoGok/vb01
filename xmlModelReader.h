#ifndef XML_MODEL_READER_H
#define XML_MODEL_READER_H

#include "modelReader.h"

namespace tinyxml2{
		class XMLElement;
}

namespace vb01{
		class Node;
		class Mesh;
		class Skeleton;
		class Bone;

		class XmlModelReader : public ModelReader{
				public:
						XmlModelReader(Model*, std::string);
				private:
						Node* processNode(Node*, tinyxml2::XMLElement*, bool = false);
						Mesh* processMesh(tinyxml2::XMLElement*);
						Skeleton* processSkeleton(Node*, tinyxml2::XMLElement*);
		};
}

#endif
