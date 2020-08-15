#include "animationChannel.h"
#include "animationController.h"
#include "animation.h"

using namespace std;

namespace vb01{
	AnimationChannel::AnimationChannel(AnimationController *controller){
		this->controller = controller;
	}

	AnimationChannel::~AnimationChannel(){
	}
}
