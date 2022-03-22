#ifndef ATTACHABLE_H
#define ATTACHABLE_H

#include <string>

namespace vb01{
		class Node;

		struct Attachable{
				std::string fullName = "";
				Node *node = nullptr;

				Attachable(std::string fullName){this->fullName = fullName;}
				Attachable(){}
				virtual void onAttached(Node*){}
				std::string getFullName(){return fullName;}
		};
}

#endif
