#ifndef IK_SOLVER_H
#define IK_SOLVER_H

#include "vector.h"

namespace vb01{
	class Bone;

	class IkSolver{
		public:
			IkSolver();
			~IkSolver(){}
			static void calculateFabrik(int, Bone**, Vector3[], Vector3);
		private:
	};
}

#endif
