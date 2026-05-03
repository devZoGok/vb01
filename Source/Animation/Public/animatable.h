#ifndef ANIMATABLE_H
#define ANIMATABLE_H

#include "keyframeChannel.h"

namespace vb01{
	class Animatable{
		public:
				enum Type{
						NONE,
						NODE,
						BONE,
						SHAPE_KEY,
						LIGHT,
						PARTICLE_EMITTER,
						MATERIAL_UNIFORM,
						TEXTURE
				};

			Animatable(Type, std::string = "");
			virtual void animate(float, KeyframeChannel){}
			inline std::string getName(){return name;}
			inline Type getType(){return type;}
		protected:
			std::string name;
			Type type = Type::NONE;
	};
}

#endif
