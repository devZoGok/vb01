#ifndef LIGHT_H
#define LIGHT_H

#include"vector.h"

namespace vb01{
	class Node;
	class Texture;
	class Shader;

	class Light{
		public:
			enum Type{POINT,DIRECTIONAL,SPOT,AMBIENT};

			Light(Type);
			~Light();
			void update();
			inline void setNode(Node *node){this->node=node;}
			inline void setPosition(Vector3 pos){this->position=pos;}
			inline void setColor(Vector3 color){this->color=color;}
			inline void setDirection(Vector3 dir){this->direction=dir;}
			inline void setAttenuationValues(float a, float b, float c){
				attenuationValues.x=a;
				attenuationValues.y=b;
				attenuationValues.z=c;
			}
			inline void setInnerAngle(float innerAngle){this->innerAngle=innerAngle;}
			inline void setOuterAngle(float outerAngle){this->outerAngle=outerAngle;}
			inline void setShadowNearPlane(float nearPlane){this->nearPlane=nearPlane;}
			inline void setShadowFarPlane(float farPlane){this->farPlane=farPlane;}
			inline Vector3 getDirection(){return direction;}
			inline Vector3 getAttenuationValues(){return attenuationValues;}
			inline float getInnerAngle(){return innerAngle;}
			inline float getOuterAngle(){return outerAngle;}
			inline float getShadowNearPlane(){return nearPlane;}
			inline float getShadowFarPlane(){return farPlane;}
			inline Node* getNode(){return node;}
		private:
			Type type;
			Shader *depthMapShader;
			Texture *depthMap=nullptr;
			Node *node=nullptr;
			Vector3 position=Vector3::VEC_ZERO,color,attenuationValues=Vector3(1.8,.7,1),direction;
			float innerAngle=.707,outerAngle=.714,nearPlane=.1,farPlane=100;
			unsigned int depthmapFBO,depthMapSize=1024;
	};
}

#endif
