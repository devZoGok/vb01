#include"camera.h"

namespace vb01{
	Camera::Camera(){
	
	}

	Camera::~Camera(){}

	void Camera::update(){
	
	}

	void Camera::lookAt(Vector3 direction, Vector3 up){
		this->direction=direction;
		this->up=up;
		left=direction.cross(up);
	}
}
