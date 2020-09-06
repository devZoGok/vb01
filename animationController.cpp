#include "animationController.h"
#include "animationChannel.h"
#include "skeleton.h"

using namespace std;

namespace vb01{
	AnimationController::AnimationController(Skeleton *skeleton){
		this->skeleton = skeleton;
	}

	AnimationController::~AnimationController(){
	}

	void AnimationController::update(){
		for(AnimationChannel *channel : channels){
			channel->update();

			Animation *animation = skeleton->getAnimationController()->getAnimation(channel->getAnimationName());
			for(Bone *channelBone : channel->getBones()){
				Animation::KeyframeGroup *keyframeGroup = animation->getKeyframeGroup(channelBone);
				int pastKeyframeId = -1;

				Vector3 pastPos, currentPos, nextPos, pastScale, currentScale, nextScale;
				Quaternion pastRot, currentRot, nextRot;

				for(KeyframeChannel animChannel : keyframeGroup->keyframeChannels){
					for(int i = 0 ; i < animChannel.keyframes.size(); i++){
						Keyframe keyframe = animChannel.keyframes[i];
						if(channel->getCurrentFrame() >= keyframe.frame){
							pastKeyframeId = i;
						}
					}
	
					Keyframe pastKeyframe = animChannel.keyframes[pastKeyframeId];
					Keyframe nextKeyframe = animChannel.keyframes[pastKeyframeId + 1];
					int pastFrame = pastKeyframe.frame;
					int nextFrame = nextKeyframe.frame;
					float ratio = (float)(channel->getCurrentFrame() - pastFrame) / (nextFrame - pastFrame);
					
					switch(animChannel.type){
						case KeyframeChannel::POS_X:
							pastPos.x = pastKeyframe.value;
							nextPos.x = nextKeyframe.value;
							break;
						case KeyframeChannel::POS_Y:
							pastPos.y = pastKeyframe.value;
							nextPos.y = nextKeyframe.value;
							break;
						case KeyframeChannel::POS_Z:
							pastPos.z = pastKeyframe.value;
							nextPos.z = nextKeyframe.value;
							break;
						case KeyframeChannel::ROT_W:
							pastRot.w = pastKeyframe.value;
							nextRot.w = nextKeyframe.value;
							break;
						case KeyframeChannel::ROT_X:
							pastRot.x = pastKeyframe.value;
							nextRot.x = nextKeyframe.value;
							break;
						case KeyframeChannel::ROT_Y:
							pastRot.y = pastKeyframe.value;
							nextRot.y = nextKeyframe.value;
							break;
						case KeyframeChannel::ROT_Z:
							pastRot.z = pastKeyframe.value;
							nextRot.z = nextKeyframe.value;
							break;
						case KeyframeChannel::SCALE_X:
							pastScale.x = pastKeyframe.value;
							nextScale.x = nextKeyframe.value;
							break;
						case KeyframeChannel::SCALE_Y:
							pastScale.y = pastKeyframe.value;
							nextScale.y = nextKeyframe.value;
							break;
						case KeyframeChannel::SCALE_Z:
							pastScale.z = pastKeyframe.value;
							nextScale.z = nextKeyframe.value;
							break;
					}

					switch(pastKeyframe.interpolation){
						case Keyframe::CONSTANT:
							currentPos = pastPos;
							currentRot = pastRot;
							currentScale = pastScale;
							break;
						case Keyframe::LINEAR:
							currentPos = pastPos + (nextPos - pastPos) * ratio;
							currentRot = pastRot + (nextRot - pastRot) * ratio;
							currentScale = pastScale + (nextScale - pastScale) * ratio;
							break;
						case Keyframe::BEZIER:
							currentPos = pastPos + (nextPos - pastPos) * ratio;
							currentRot = pastRot + (nextRot - pastRot) * ratio;
							currentScale = pastScale + (nextScale - pastScale) * ratio;
							break;
					}
				}
				/*
				swap(currentPos.y, currentPos.z);
				currentPos.z = -currentPos.z;
				swap(currentRot.y, currentRot.z);
				currentRot.z = -currentRot.z;
				*/

				channelBone->setPosePos(currentPos);
				channelBone->setPoseRot(currentRot);
				//channelBone->setPoseScale(currentScale);
			}
		}
	}

	Animation* AnimationController::getAnimation(string name){
		Animation *anim = nullptr;
		for(Animation *a : animations)
			if(a->getName() == name){
				anim = a;
				break;
			}
		return anim;
	}
}
