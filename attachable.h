#ifndef ATTACHABLE_H
#define ATTACHABLE_H

#include <string>

namespace vb01{
		class Node;

		struct Attachable{
				public:
					Attachable(std::string fullName){this->fullName = fullName;}
					Attachable(){}
					virtual void onAttached(Node *node){this->node = node;}
					std::string getAttachableName(){return fullName;}
				protected:
					std::string fullName = "";
					Node *node = nullptr;
		};
}

#endif
