#ifndef QUAD_H
#define QUAD_H

#include "vector.h"
#include "mesh.h"

namespace vb01{
	class Quad : public Mesh{
		public:
			Quad(Vector3, bool = true, int numVertDiv = 0, int numHorDiv = 0);
			void setSize(Vector3);
			inline Vector3 getSize(){return size;}
		private:
			Vector3 size;
			bool spatial;
			int numVertDiv, numHorDiv;
	};
}

#endif
