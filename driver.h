#ifndef DRIVER_H
#define DRIVER

#include "keyframeChannel.h"

namespace vb01{
	class Animatable;

	class Driver{
		public:
			enum VariableType{ 
				POS_X,
			   	POS_Y,
			   	POS_Z,
			   	ROT_W,
			   	ROT_X,
			   	ROT_Y,
			   	ROT_Z,
			   	SCALE_X,
			   	SCALE_Y,
			   	SCALE_Z
		   	};

			Driver();
			inline VariableType getType(){return type;}
			void drive(float);
		private:
			Animatable *target; 
			VariableType type;
			KeyframeChannel keyframeChannel;
	};
}

#endif
