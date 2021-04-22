#include "animationChannelTest.h"
#include "skeleton.h"
#include "animationController.h"
#include "animationChannel.h"
#include "animation.h"
#include "bone.h"

#include <vector>

using namespace std;

namespace vb01{
	void AnimationChannelTest::setUp(){
		Bone *bone = new Bone("bone", 1, Vector3(0, 0, 0), Quaternion::QUAT_I, Vector3::VEC_IJK);
		skeleton = new Skeleton(new AnimationController());
		skeleton->addBone(bone, nullptr);
		AnimationController *controller = skeleton->getAnimationController();

		Keyframe k1, k2;
		k1.frame = 0;
		k2.frame = 10;
		KeyframeChannel kc;
		kc.type = KeyframeChannel::Type::POS_X;
		kc.keyframes = vector<Keyframe>({k1, k2});
		kc.animatable = bone;
		Animation *anim = new Animation("anim");
		anim->addKeyframeChannel(kc);

		channel = new AnimationChannel();
		controller->addAnimationChannel(channel);
		controller->addAnimation(anim);
		channel->addAnimatable(bone);
		channel->setAnimationName("anim");
	}

	void AnimationChannelTest::tearDown(){
	}

	void AnimationChannelTest::testUpdate(){
		int numFrames = 10;
		int firstFrame = channel->getFirstFrame();
		channel->numFrames = numFrames;
		
		channel->lastUpdateTime = 0;
		channel->setLoop(false);
		channel->setForward(true);
		channel->update();
		CPPUNIT_ASSERT(channel->getCurrentFrame() == firstFrame + 1);

		channel->lastUpdateTime = 0;
		channel->setCurrentFrame(numFrames - 1);
		channel->setLoop(false);
		channel->setForward(true);
		channel->update();
		CPPUNIT_ASSERT(channel->getCurrentFrame() == numFrames);

		channel->lastUpdateTime = 0;
		channel->setCurrentFrame(1);
		channel->setLoop(false);
		channel->setForward(false);
		channel->update();
		CPPUNIT_ASSERT(channel->getCurrentFrame() == firstFrame);

		channel->lastUpdateTime = 0;
		channel->setCurrentFrame(numFrames);
		channel->setLoop(true);
		channel->setForward(true);
		channel->update();
		CPPUNIT_ASSERT(channel->getCurrentFrame() == firstFrame);

		channel->lastUpdateTime = 0;
		channel->setCurrentFrame(firstFrame);
		channel->setLoop(true);
		channel->setForward(false);
		channel->update();
		CPPUNIT_ASSERT(channel->getCurrentFrame() == numFrames);
	}

	void AnimationChannelTest::testGetMaxKeyframeNum(){
		int numFrames = channel->getMaxKeyframeNum("anim");
		CPPUNIT_ASSERT(numFrames == 10);
	}

	void AnimationChannelTest::testGetFirstFrame(){
		int firstFrame = channel->getFirstFrameNum("anim");
		CPPUNIT_ASSERT(firstFrame == 0);
	}
}
