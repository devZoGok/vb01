#include "vector.h"
#include "util.h"

#include <vector>

namespace vb01{
	class Node;
	class Mesh;

	class Ray{
		public:
			struct CollisionResult{
				Vector3 pos;
				float distance;
				Mesh *mesh = nullptr;
			};
			static void retrieveCollisions(Vector3, Vector3, Node*, std::vector<CollisionResult>&, const float = .0);
			static void castRay(Vector3, Vector3, Node*, std::vector<CollisionResult>&, const float);
			static void sortResults(std::vector<CollisionResult>&);
	};

}
