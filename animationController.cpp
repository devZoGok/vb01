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

			transformBones(channel);
		}
	}

	void AnimationController::transformBones(AnimationChannel *channel){
		Animation *animation = getAnimation(channel->getAnimationName());

		for(Bone *channelBone : channel->getBones()){
			Animation::KeyframeGroup *keyframeGroup = animation->getKeyframeGroup(channelBone);
			Vector3 pastPos, nextPos, pastScale, nextScale;
			Quaternion pastRot, nextRot;
			Keyframe::Interpolation interp;
			float ratio;

			prepareAdjacentValues(pastPos, nextPos, pastRot, nextRot, pastScale, nextScale, interp, ratio, keyframeGroup, channel);

			Vector3 currentPos = interpolate(pastPos, nextPos, interp, ratio);
			Quaternion currentRot = interpolate(pastRot, nextRot, interp, ratio);
			Vector3 currentScale = interpolate(pastScale, nextScale, interp, ratio);
			channelBone->setPosePos(currentPos);
			channelBone->setPoseRot(currentRot);
		}
	}

	void AnimationController::prepareAdjacentValues(
			Vector3 &pastPos,
		   	Vector3 &nextPos,
		   	Quaternion &pastRot,
		   	Quaternion &nextRot,
		   	Vector3 &pastScale,
		   	Vector3 &nextScale,
			KeyframeInterpolation &interpMode,
			float &ratio,
			Animation::KeyframeGroup *keyframeGroup,
			AnimationChannel *channel
			){

		for(KeyframeChannel keyframeChannel : keyframeGroup->keyframeChannels){
			int pastKeyframeId = -1, nextKeyframeId = -1;
			for(int i = 0; i < keyframeChannel.keyframes.size(); i++){
				Keyframe keyframe = keyframeChannel.keyframes[i];
				if(channel->getCurrentFrame() >= keyframe.frame){
					pastKeyframeId = i;
				}
			}
			nextKeyframeId = pastKeyframeId + 1;
	
			Keyframe pastKeyframe = keyframeChannel.keyframes[pastKeyframeId];
			Keyframe nextKeyframe = keyframeChannel.keyframes[nextKeyframeId];
			int pastFrame = pastKeyframe.frame;
			int nextFrame = nextKeyframe.frame;
			ratio = (float)(max(0, channel->getCurrentFrame() - pastFrame)) / (nextFrame - pastFrame);
			interpMode = pastKeyframe.interpolation;
			
			switch(keyframeChannel.type){
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
		}
	}

	Vector3 AnimationController::interpolate(Vector3 pastPos, Vector3 nextPos, Keyframe::Interpolation mode, float ratio){
		Vector3 currentPos;
		switch(mode){
			case Keyframe::CONSTANT:
				currentPos = pastPos;
				break;
			case Keyframe::LINEAR:
				currentPos = pastPos + (nextPos - pastPos) * ratio;
				break;
			case Keyframe::BEZIER:
				currentPos = pastPos + (nextPos - pastPos) * ratio;
				break;
		}
		return currentPos;
	}

	Quaternion AnimationController::interpolate(Quaternion pastRot, Quaternion nextRot, Keyframe::Interpolation mode, float ratio){
		Quaternion currentRot;
		switch(mode){
			case Keyframe::CONSTANT:
				currentRot = pastRot;
				break;
			case Keyframe::LINEAR:
				currentRot = pastRot + (nextRot - pastRot) * ratio;
				break;
			case Keyframe::BEZIER:
				currentRot = pastRot + (nextRot - pastRot) * ratio;
				break;
		}
		return currentRot;
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
