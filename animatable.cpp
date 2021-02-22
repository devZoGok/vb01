#include "animatable.h"
#include "animationController.h"

namespace vb01{
	Animatable::Animatable(AnimationController *controller){
		this->animationController = controller;
	}

	void Animatable::update(){
		if(animationController)
			animationController->update();
	}
}
