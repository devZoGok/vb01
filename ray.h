#include"vector.h"
#include"util.h"
#include<vector>

namespace vb01{
	class Node;
	class Mesh;

	struct CollisionResult{
		Vector3 pos;
		float distance;
		Mesh *mesh=nullptr;
	};

	void retrieveCollisions(Vector3,Vector3,Node*,std::vector<CollisionResult>&,const float=.0);
	void sortResults(std::vector<CollisionResult>&);
}
