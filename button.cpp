#include <locale>
#include <codecvt>
#include <glfw3.h>

#include "root.h"
#include "quad.h"
#include "node.h"
#include "material.h"
#include "text.h"
#include "texture.h"
#include "util.h"
#include "button.h"

using namespace std;
using namespace vb01;

namespace vb01Gui{
	Button::Button(Vector3 p, Vector2 s, string n, string fontPath, int t, bool sep, string ip) : 
		pos(p), 
		size(s), 
		name(n), 
		trigger(t), 
		separate(sep), 
		imagePath(ip),
		textOffset(Vector3(0, s.y, .1))
	{
		Root *root = Root::getSingleton();
		guiNode = root->getGuiNode();
		rect = new Quad(Vector3(size.x, size.y, 0), false);
		rectNode = new Node(pos);
		Material *mat = new Material(root->getLibPath() + "gui");

		if(imagePath == ""){
			mat->addBoolUniform(texUni, false);
			mat->addVec4Uniform(diffColUni, color);
		}
		else{
			string frame[]{imagePath};
			Texture *tex = new Texture(frame, 1, false);
			textures.push_back(tex);

			mat->addTexUniform("diffuseMap", tex, false);
			mat->addBoolUniform(texUni, true);
		}

		rect->setMaterial(mat);
		rectNode->attachMesh(rect);
		guiNode->attachChild(rectNode);	

		if(separate && fontPath != ""){
			text = new Text(fontPath, stringToWstring(name));

			float sc = .2;
			textNode = new Node(pos + textOffset, Quaternion::QUAT_W, Vector3(sc, sc, 1));
			textNode->addText(text);

			Material *textMat = new Material(root->getLibPath() + "text");
			textMat->addBoolUniform(texUni, false);
			textMat->addVec4Uniform(diffColUni, Vector4::VEC_IJKL);
			text->setMaterial(textMat);

			guiNode->attachChild(textNode);
		}

		initWindowSize[0] = Root::getSingleton()->getWidth();
		initWindowSize[1] = Root::getSingleton()->getHeight();
	}

	Button::~Button(){
		if(textNode){
			guiNode->dettachChild(textNode);
			delete textNode;
		}

		guiNode->dettachChild(rectNode);
		delete rectNode;
	}

	void Button::update(){
		int width, height;
		GLFWwindow *window = Root::getSingleton()->getWindow();
		glfwGetWindowSize(window, &width, &height);

		float posRatio[] = {pos.x / initWindowSize[0], pos.y / initWindowSize[1]};
		float sizeRatio[] = {size.x / initWindowSize[0], size.y / initWindowSize[1]};

		pos = Vector3(width * posRatio[0], height * posRatio[1], pos.z);
		size = Vector2(width * sizeRatio[0], height * sizeRatio[1]);

		initWindowSize[0] = width;
		initWindowSize[1] = height;

		if(textNode)
			textNode->setVisible(active);

		rectNode->setVisible(active);
	}

	void Button::onMouseOver(){
		mouseOver = true;
	}

	void Button::onMouseOff(){
		mouseOver = false;
	}

	void Button::setPos(Vector3 pos){
		this->pos = pos;

		rectNode->setPosition(pos);

		if(textNode)
			textNode->setPosition(pos + textOffset);
	}

	void Button::setSize(Vector2 size){
		this->size = size;
		rect->setSize(Vector3(size.x, size.y, 0));
	}

	void Button::setColor(Vector4 c){
		this->color = c;
		((Material::Vector4Uniform*)rect->getMaterial()->getUniform(diffColUni))->value = color;
	}
	
	void Button::setImage(string image){
		int texId = -1;
		Material *mat = rect->getMaterial();

		for(int i = 0; i < textures.size(); i++)
			if(textures[i]->getPath()[0] == image)
				texId = i;

		if(texId == -1){
			string p[]{image};
			textures.push_back(new Texture(p, 1, false));
			mat->setTexUniform("diffuseMap", textures[textures.size() - 1], false);
		}
		else
			mat->setTexUniform("diffuseMap", textures[texId], false);

		this->imagePath = image;
	}
}
