#include "vector.h"
#include "util.h"

#include <vector>

namespace vb01{
	class Node;
	class Mesh;

	class RayCaster{
		public:
			struct CollisionResult{
				Vector3 pos, norm;
				float distance;
				Mesh *mesh = nullptr;
			};

			static std::vector<CollisionResult> cast(Vector3, Vector3, Node*, const float = .0);
			static std::vector<CollisionResult> cast(Vector3, Vector3, std::vector<Node*>, const float = .0);
		private:
			static std::vector<CollisionResult> retrieveCollisions(Vector3, Vector3, Node*, const float);
			static void sortResults(std::vector<CollisionResult>&);
	};

}
