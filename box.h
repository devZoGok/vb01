#ifndef BOX_H
#define BOX_H

#include"mesh.h"

namespace vb01{
	class Box : public Mesh{
		public:
			Box(Vector3);
			void setSize(Vector3);
		private:
			Vector3 size;
	};
}

#endif
