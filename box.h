#ifndef BOX_H
#define BOX_H

#include "mesh.h"

namespace vb01{
	class Box : public Mesh{
		public:
			Box(Vector3);
			inline Box* clone(){return new Box(this);}
			inline void setSize(Vector3 size){this->size = size;}
			inline Vector3 getSize(){return size;}
		private:
			Box(Box*);
			void init();

			Vector3 size;
	};
}

#endif
