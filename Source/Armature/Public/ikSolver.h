#ifndef IK_SOLVER_H
#define IK_SOLVER_H

#include "vector.h"

namespace vb01{
	class Bone;

	class IkSolver{
		public:
			static void calculateFabrik(int, Bone**, Vector3[], Vector3);
	};
}

#endif
