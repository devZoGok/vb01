#ifndef DRIVER_H
#define DRIVER

#include "keyframeChannel.h"

#include <string>

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

			Driver(KeyframeChannel, VariableType);
			void drive(float);
			static VariableType getDriverVariableType(std::string);  
			inline VariableType getType(){return type;}
			inline KeyframeChannel& getKeyframeChannel(){return keyframeChannel;}
		private:
			VariableType type;
			KeyframeChannel keyframeChannel;
	};
}

#endif
