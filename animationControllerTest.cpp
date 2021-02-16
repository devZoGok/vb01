#include "animationControllerTest.h"
#include "skeleton.h"
#include "animation.h"
#include "animationController.h"
#include "animationChannel.h"

#include <vector>

using namespace std;

namespace vb01{
	void AnimationControllerTest::createAnimation(){
		Keyframe k0;
		k0.frame = 0;
		k0.value = 1;
		k0.interpolation = KeyframeInterpolation::LINEAR;
		Keyframe k1;
		k1.frame = 1;
		k1.value = 0;
		k1.interpolation = KeyframeInterpolation::LINEAR;

		KeyframeChannel ch0;
		ch0.type = KeyframeChannelType::POS_X;
		ch0.keyframes = vector<Keyframe>({k0});
		KeyframeChannel ch1;
		ch1.type = KeyframeChannelType::POS_Y;
		ch1.keyframes = vector<Keyframe>({k1});

		Bone *bone = skeleton->getBone(0);
		KeyframeGroup kg;
		kg.bone = bone;
		kg.keyframeChannels = vector<KeyframeChannel>({ch0, ch1});

		controller = skeleton->getAnimationController();
		Animation *anim = new Animation("anim");
		anim->addKeyframeGroup(kg);
		controller->addAnimation(anim);

		channel = new AnimationChannel(controller);
		channel->addBone(bone);
		channel->setAnimationName("anim");
		channel->setLoop(true);
		controller->addAnimationChannel(channel);
	}

	void AnimationControllerTest::setUp(){
		Node *rootNode = Root::getSingleton()->getRootNode();
		skeleton = new Skeleton(new AnimationController());
		Bone *bone = new Bone("bone", 1.f, Vector3::VEC_ZERO, Quaternion::QUAT_W, Vector3::VEC_IJK);
		skeleton->addBone(bone, (Bone*)rootNode);

		createAnimation();
	}


	void AnimationControllerTest::tearDown(){
	}

	void AnimationControllerTest::testPrepareAdjacentValues(){
		channel->currentFrame = 4;
		controller->update();
		Bone *bone = skeleton->getBone("bone");
		Vector3 posePos = bone->getPosePos();

		/*
		CPPUNIT_ASSERT(posePos.y == 0);
		CPPUNIT_ASSERT(posePos.x == 1);
		*/
	}
}
