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
			Animation *animation = skeleton->getAnimationController()->getAnimation(channel->getAnimationName());
			for(Bone *channelBone : channel->getBones()){
				Animation::KeyframeGroup *keyframeGroup = animation->getKeyframeGroup(channelBone);
				int keyframeId = -1;
				for(int i = 0 ; i < keyframeGroup->keyframes.size(); i++){
					Animation::KeyframeGroup::Keyframe keyframe = keyframeGroup->keyframes[i];
					if(keyframe.frame > channel->getCurrentFrame()){
						keyframeId = i;
						break;
					}
				}


				Animation::KeyframeGroup::Keyframe pastKeyframe = keyframeGroup->keyframes[keyframeId - 1];
				int pastFrame = pastKeyframe.frame;
				int nextFrame = keyframeGroup->keyframes[keyframeId].frame;
				float ratio = (float)(channel->getCurrentFrame() - pastFrame) / (nextFrame - pastFrame);
				
				switch(pastKeyframe.interpolation){
					case Animation::KeyframeGroup::Keyframe::CONSTANT:
						break;
					case Animation::KeyframeGroup::Keyframe::LINEAR:
						break;
					case Animation::KeyframeGroup::Keyframe::BEZIER:
						break;
				}
			}
		}
	}

	void AnimationController::applyBoneTransforms(Animation::KeyframeGroup *keyframeGroup){
		Bone *bone = keyframeGroup->bone;
		Vector3 pos, scale;
		Quaternion rot;
		for(Animation::KeyframeGroup::Keyframe keyframe : keyframeGroup->keyframes){
			switch(keyframe.type){
				case Animation::KeyframeGroup::Keyframe::POS_X:
					pos.x = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::POS_Y:
					pos.y = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::POS_Z:
					pos.z = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::ROT_W:
					rot.x = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::ROT_X:
					rot.x = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::ROT_Y:
					rot.x = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::ROT_Z:
					rot.x = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::SCALE_X:
					scale.x = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::SCALE_Y:
					scale.y = keyframe.value;
					break;
				case Animation::KeyframeGroup::Keyframe::SCALE_Z:
					scale.z = keyframe.value;
					break;
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
