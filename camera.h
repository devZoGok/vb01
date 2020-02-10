#ifndef CAMERA_H
#define CAMERA_H

#include"vector.h"

namespace vb01{
	class Camera{
		public:
			Camera();
			~Camera();
			void update();
			void setPosition(Vector3 position){this->position=position;}
			void lookAt(Vector3, Vector3=Vector3(0,1,0));
			inline Vector3 getPosition(){return position;}
			inline Vector3 getDirection(){return direction;}
			inline Vector3 getLeft(){return left;}
			inline Vector3 getUp(){return up;}
			inline float getFov(){return fov;}
			inline float getNearPlane(){return nearPlane;}
			inline float getFarPlane(){return farPlane;}
		private:
			Vector3 position=Vector3(0,0,0),direction=Vector3(0,0,-1),left=Vector3(1,0,0),up=Vector3(0,1,0);
			float fov=45,nearPlane=.1,farPlane=100;
	};
}

#endif
