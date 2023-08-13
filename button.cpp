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
	Button::Button(Vector2 pos, Vector2 size, string name, string fontPath, int trigger, bool separate, string imagePath){
		this->pos = pos;
		this->size = size;
		this->name = name;
		this->separate = separate;
		this->imagePath = imagePath;
		this->trigger = trigger;

		Root *root = Root::getSingleton();
		guiNode = root->getGuiNode();
		rect = new Quad(Vector3(size.x, size.y, 0), false);
		rectNode = new Node(Vector3(pos.x, pos.y, 0));
		Material *mat = new Material(root->getLibPath() + "gui");

		if(imagePath == ""){
			mat->addBoolUniform(texUni, false);
			mat->addVec4Uniform(diffColUni, color);
		}
		else{
			mat->addBoolUniform(texUni, true);
			string frame[]{imagePath};
			Texture *tex = new Texture(frame, 1, false);
			mat->addTexUniform("diffuseMap", tex, false);
			textures.push_back(tex);
		}

		rect->setMaterial(mat);
		rectNode->attachMesh(rect);
		guiNode->attachChild(rectNode);	

		if(separate){
			text = new Text(fontPath, stringToWstring(name));
			text->setScale(.2);

			textNode = new Node(Vector3(pos.x, pos.y + size.y, - .1));
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

			pos = Vector2(width * posRatio[0], height * posRatio[1]);
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

	void Button::setPos(Vector2 pos){
		this->pos = pos;
		float z1 = rectNode->getPosition().z;
		rectNode->setPosition(Vector3(pos.x, pos.y, z1));

		if(textNode){
			float z2 = textNode->getPosition().z;
			textNode->setPosition(Vector3(pos.x, pos.y + size.y, z2));
		}
	}

	void Button::setSize(Vector2 size){
		this->size = size;
		rect->setSize(Vector3(size.x, size.y, 0));
	}

	void Button::setColor(Vector4 c){
		this->color = c;
		((Material::Vector4Uniform*)rect->getMaterial()->getUniform(diffColUni))->value = color;
	}
	
	void Button::setZOrder(float zOrder){
		Vector3 rectPos = rectNode->getPosition();
		rectNode->setPosition(Vector3(rectPos.x, rectPos.y, zOrder));

		if(textNode){
			Vector3 textPos = textNode->getPosition();	
			textNode->setPosition(Vector3(textPos.x, textPos.y, zOrder - .1));
		}
	}

	void Button::setImage(string image){
		int texId = -1;
		Material *mat = rect->getMaterial();

		for(int i = 0; i < textures.size(); i++)
			if(textures[i]->getPath()[0] == image)
				texId = i;

		/*
		if(texId == -1){
			string p[]{image};
			textures.push_back(new Texture(p, 1));
			mat->setDiffuseMap(textures[textures.size() - 1], 0);
		}
		else
			mat->setDiffuseMap(textures[texId], 0);
			*/

		this->imagePath = image;
	}
}
