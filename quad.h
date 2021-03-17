#ifndef QUAD_H
#define QUAD_H

#include "vector.h"
#include "mesh.h"

namespace vb01{
	class Quad : public Mesh{
		public:
			Quad(Vector3, bool = true);
			void setSize(Vector3);
		private:
			Vector3 size;
			bool spatial;
	};
}

#endif
