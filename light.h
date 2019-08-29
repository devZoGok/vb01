#ifndef LIGHT_H
#define LIGHT_H

#include"vector.h"

namespace vb01{
	class Node;

	class Light{
		public:
			enum Type{POINT,DIRECTIONAL,SPOT,AMBIENT};

			Light();
			~Light();
			void update();
			inline void setNode(Node *node){this->node=node;}
			inline void setPosition(Vector3 pos){this->pos=pos;}
			inline void setColor(Vector3 color){this->color=color;}
			inline void setDirection(Vector3 dir){this->direction=dir;}
			inline void setAttenuationValues(float a, float b, float c){
				attenuationValues.x=a;
				attenuationValues.y=b;
				attenuationValues.z=c;
			}
			inline void setType(Type t){this->type=t;}
			inline void setInnerAngle(float innerAngle){this->innerAngle=innerAngle;}
			inline void setOuterAngle(float outerAngle){this->outerAngle=outerAngle;}
			inline Vector3 getDirection(){return direction;}
			inline Vector3 getAttenuationValues(){return attenuationValues;}
			inline float getInnerAngle(){return innerAngle;}
			inline float getOuterAngle(){return outerAngle;}
		private:
			Type type;
			Node *node=nullptr;
			Vector3 pos,color,attenuationValues=Vector3(1.8,.7,1),direction;
			float innerAngle=.707,outerAngle=.714;
	};
}

#endif
