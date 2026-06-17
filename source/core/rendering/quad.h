#ifndef QUAD_H
#define QUAD_H

#include "vector.h"
#include "mesh.h"

namespace vb01{
	class Quad : public Mesh{
		public:
			Quad(Vector3, bool = true, int numVertDiv = 0, int numHorDiv = 0);
			void setSize(Vector3);
			Vector3 getSubquadCorner(int, int, int, int, bool, bool);
			Vector2 getSubquadIds(int, int, Vector3, Vector3);
			inline Vector3 getSize(){return size;}
			inline Vector2 getSubquadSize(){return Vector2(size.x / (numHorDiv + 1), size.y / (numVertDiv + 1));}
			inline int getNumHorSubquads(){return numHorDiv + 1;}
			inline int getNumVertSubquads(){return numVertDiv + 1;}
		private:
			Vector3 size;
			bool spatial;
			int numVertDiv, numHorDiv;
	};
}

#endif
