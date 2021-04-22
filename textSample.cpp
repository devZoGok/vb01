#include <string>

#include "root.h"
#include "text.h"
#include "material.h"
#include "node.h"
#include "animationController.h"
#include "animationChannel.h"

using namespace std;
using namespace vb01;

int main(){
	Root *root = Root::getSingleton();
	Node *guiNode = root->getGuiNode();
	const string PATH = "../", FONT_PATH = PATH + "samples/textSample/fonts/", TEX_PATH = PATH + "samples/textures/";
	root->start(800, 600, PATH, "Text sample");

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
	
	Material *textMat = new Material(Material::MATERIAL_TEXT);
	textMat->setTexturingEnabled(true);
	string frames[]{TEX_PATH + "bricks.jpg", TEX_PATH + "defaultTexture.jpg"};
	Texture *texture = new Texture(frames, 2);
	textMat->addDiffuseMap(texture);

	for(int i = 0; i < numTexts; i++){
		Text *text = new Text(FONT_PATH + fonts[i], texts[i]);
		text->setLeftToRight(leftToRight[i]);
		text->setScale(.5);
		text->setHorizontal(horizontal[i]);
		text->setMaterial(textMat);
		Node *textNode = new Node(Vector3(0, 50 * (i + 1), 0), Quaternion::QUAT_W, Vector3::VEC_IJK, "", new AnimationController());
		textNode->addText(text);
		guiNode->attachChild(textNode);

		KeyframeChannel kcA;
		kcA.animatable = texture;
		kcA.type = KeyframeChannel::TEXTURE_FRAME_A;
		kcA.keyframes = vector<Keyframe>({KeyframeChannel::createKeyframe(Keyframe::CONSTANT, 0, 1)});
		
		KeyframeChannel kcB;
		kcB.animatable = texture;
		kcB.type = KeyframeChannel::TEXTURE_FRAME_B;
		Keyframe k1;
		k1.interpolation = Keyframe::CONSTANT;
		k1.value = 1;
		k1.frame = 1;
		kcB.keyframes = vector<Keyframe>({k1});
		
		KeyframeChannel kcC;
		kcC.animatable = texture;
		kcC.type = KeyframeChannel::TEXTURE_MIX_RATIO;
		Keyframe k;
		k.interpolation = Keyframe::LINEAR;
		k.value = 0;
		k.frame = 1;
		Keyframe kk;
		kk.interpolation = Keyframe::LINEAR;
		kk.value = 1;
		kk.frame = 15;
		Keyframe kkk;
		kkk.interpolation = Keyframe::LINEAR;
		kkk.value = 0;
		kkk.frame = 29;
		kcC.keyframes = vector<Keyframe>({k, kk, kkk});
		Animation *anim = new Animation("tex");
		anim->addKeyframeChannel(kcA);
		anim->addKeyframeChannel(kcB);
		anim->addKeyframeChannel(kcC);

		AnimationController *controller = textNode->getAnimationController();
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
