#include "animationController.h"
#include "animationChannel.h"
#include "skeleton.h"

using namespace std;

namespace vb01{
	AnimationController::AnimationController(){
	}

	AnimationController::~AnimationController(){
	}

	void AnimationController::update(){
		for(AnimationChannel *channel : channels){
			channel->update();

			transform(channel);
		}
	}

	Keyframe AnimationController::findKeyframe(AnimationChannel *channel, KeyframeChannel keyframeChannel, bool last){
		int pastKeyframeId = -1;
		for(int i = 0; i < keyframeChannel.keyframes.size(); i++){
			Keyframe keyframe = keyframeChannel.keyframes[i];
			if(channel->getCurrentFrame() >= keyframe.frame){
				pastKeyframeId = i;
			}
		}
		return keyframeChannel.keyframes[pastKeyframeId + (last ? 0 : 1)];
	}

	void AnimationController::transform(AnimationChannel *channel){
		vector<Node*> transformNodes;
		if(node)
			transformNodes.push_back(node);
		for(Bone *channelBone : channel->getBones())
			transformNodes.push_back((Node*)channelBone);

		Animation *animation = getAnimation(channel->getAnimationName());
		for(Node *transformNode : transformNodes){
			Vector3 currentPos;
			Quaternion currentRot;
			Vector3 currentScale;
			bool objectNode = node && node->getName() == transformNode->getName();
			if(objectNode){
				currentPos = transformNode->getPosition();
				currentRot = transformNode->getOrientation();
				currentScale = transformNode->getScale();
			}
			else{
				Bone *channelBone = (Bone*)transformNode;
				currentPos = channelBone->getPosePos();
				currentRot = channelBone->getPoseRot();
				currentScale = channelBone->getPoseScale();
			}

			vector<KeyframeChannel> keyframeChannels = animation->getKeyframeChannelsByNode(transformNode);
			for(KeyframeChannel keyframeChannel : keyframeChannels){
				Keyframe pastKeyframe = findKeyframe(channel, keyframeChannel, true);
				Keyframe nextKeyframe = findKeyframe(channel, keyframeChannel, false);
				int pastFrame = pastKeyframe.frame;
				int nextFrame = nextKeyframe.frame;

				Keyframe::Interpolation interp = pastKeyframe.interpolation;
				float ratio = (float)(max(0, channel->getCurrentFrame() - pastFrame)) / (nextFrame - pastFrame);
				float value = interpolate(pastKeyframe.value, nextKeyframe.value, interp, ratio);
				setFrameValue(value, keyframeChannel.type, currentPos, currentRot, currentScale);
			}

			if(objectNode){
				transformNode->setPosition(currentPos);
				transformNode->setOrientation(currentRot);
				transformNode->setScale(currentScale);
			}
			else{
				Bone *channelBone = (Bone*)transformNode;
				channelBone->setPosePos(currentPos);
				channelBone->setPoseRot(currentRot);
				channelBone->setPoseScale(currentScale);
			}
		}
	}

	void AnimationController::setFrameValue(float value, KeyframeChannelType type, Vector3 &currentPos, Quaternion &currentRot, Vector3 &currentScale){
		switch(type){
			case KeyframeChannel::POS_X:
				currentPos.x = value;
				break;
			case KeyframeChannel::POS_Y:
				currentPos.y = value;
				break;
			case KeyframeChannel::POS_Z:
				currentPos.z = value;
				break;
			case KeyframeChannel::ROT_W:
				currentRot.w = value;
				break;
			case KeyframeChannel::ROT_X:
				currentRot.x = value;
				break;
			case KeyframeChannel::ROT_Y:
				currentRot.y = value;
				break;
			case KeyframeChannel::ROT_Z:
				currentRot.z = value;
				break;
			case KeyframeChannel::SCALE_X:
				currentScale.x = value;
				break;
			case KeyframeChannel::SCALE_Y:
				currentScale.y = value;
				break;
			case KeyframeChannel::SCALE_Z:
				currentScale.z = value;
				break;
			case KeyframeChannel::VALUE:
				break;
		}
	}

	float AnimationController::interpolate(float pastValue, float nextValue, Keyframe::Interpolation mode, float ratio){
		float currentValue;
		switch(mode){
			case Keyframe::CONSTANT:
				currentValue = pastValue;
				break;
			case Keyframe::LINEAR:
				currentValue = pastValue + (nextValue - pastValue) * ratio;
				break;
			case Keyframe::BEZIER:
				currentValue = pastValue + (nextValue - pastValue) * ratio;
				break;
		}
		return currentValue;
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
