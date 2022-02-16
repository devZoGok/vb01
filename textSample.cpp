#include <string>

#include "root.h"
#include "text.h"
#include "material.h"
#include "node.h"
#include "animationController.h"
#include "animationChannel.h"
#include "imageReader.h"
#include "fontReader.h"
#include "assetManager.h"

using namespace std;
using namespace vb01;

int main(){
	const string PATH = "../", FONT_PATH = PATH + "samples/textSample/fonts/", TEX_PATH = PATH + "samples/textures/";

	/*
	 * Gets Root object to start the sample, 
	 * also passes the base path for asset retrieval. 
	*/
	Root *root = Root::getSingleton();
	root->start(800, 600, PATH, "Text sample");
	Node *guiNode = root->getGuiNode();

	int numTexts = 4;
	wstring texts[] = {
		L"대한민국",
		L"ĄČĘĖĮŠŲŪ„“",
		L"عشر",
		L"中華民國"
	}; 
	string fonts[] = {
		"batang.ttf",
		"Palem-nm.ttf",
		"Amiri-Regular.ttf",
		"sazanami-mincho.ttf"
	};
	bool leftToRight[] = {
		true,
		true,
		false,
		true
	};
	bool horizontal[] = {
		true,
		true,
		true,
		false
	};
	
	/*
	 * Creates a material for text objects and
	 * attaches a diffuse texture with 2 images
	*/
	Material *textMat = new Material(PATH + "text");
	textMat->addBoolUniform("texturingEnabled", true);

	string frames[]{TEX_PATH + "bricks.jpg", TEX_PATH + "defaultTexture.jpg"};
	AssetManager::getSingleton()->load(frames[0]);
	AssetManager::getSingleton()->load(frames[1]);
	Texture *texture = new Texture(frames, 2, false);
	textMat->addTexUniform("textures[0]", texture, true);

	for(int i = 0; i < numTexts; i++){
		AssetManager::getSingleton()->load(FONT_PATH + fonts[i]);

		Text *text = new Text(FONT_PATH + fonts[i], texts[i], 0, 60000);
		text->setLeftToRight(leftToRight[i]);
		text->setScale(.5);
		text->setHorizontal(horizontal[i]);
		text->setMaterial(textMat);

		Node *textNode = new Node(Vector3(0, 50 * (i + 1), 0), Quaternion::QUAT_W, Vector3::VEC_IJK, "");
		textNode->addText(text);
		guiNode->attachChild(textNode);

		/*
		 * KeyframeChannel structs determine what object and how it will be animated.
		 * They also use keyframes which in turn are determined by their interpolation types,
		 * values that are returned for animatable parameters, e.g. x coordinate for position 
		 * and frame number.
		*/
		KeyframeChannel kcA;
		kcA.animatable = texture;
		kcA.type = KeyframeChannel::TEXTURE_FRAME_A;
		kcA.keyframes = vector<Keyframe>({KeyframeChannel::createKeyframe(Keyframe::CONSTANT, 0, 1)});
		
		KeyframeChannel kcB;
		kcB.animatable = texture;
		kcB.type = KeyframeChannel::TEXTURE_FRAME_B;
		kcB.keyframes = vector<Keyframe>({KeyframeChannel::createKeyframe(Keyframe::CONSTANT, 1, 1)});
		
		KeyframeChannel kcC;
		kcC.animatable = texture;
		kcC.type = KeyframeChannel::TEXTURE_MIX_RATIO;
		kcC.keyframes = vector<Keyframe>({
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 0, 1),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 1, 15),
			KeyframeChannel::createKeyframe(Keyframe::LINEAR, 0, 29)
		});

		Animation *anim = new Animation("tex");
		anim->addKeyframeChannel(kcA);
		anim->addKeyframeChannel(kcB);
		anim->addKeyframeChannel(kcC);

		/*
		 * The AnimationController plays the animations set in the AnimationChannel objects.
		 * AnimationChannel objects require an animation and animatable objects, e.g. textures to work.
		*/
		AnimationController *controller = AnimationController::getSingleton();
		controller->addAnimation(anim);
		AnimationChannel *channel = new AnimationChannel();
		controller->addAnimationChannel(channel);
		channel->addAnimatable(texture);
		channel->setAnimationName("tex");
		channel->setLoop(true);
	}

	while(true){
		root->update();
	}

	return 0;
}
