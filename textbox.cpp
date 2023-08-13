#include <algorithm>
#include <sstream>
#include <ostream>

#include "textbox.h"
#include "slider.h"
#include "root.h"
#include "node.h"
#include "quad.h"
#include "material.h"
#include "util.h"

using namespace vb01;
using namespace std;

namespace vb01Gui{
	Textbox::TextboxButton::TextboxButton(Textbox *t, Vector2 pos, Vector2 size, string name) : Button(pos, size, name, "", -1, false) {textbox = t;}

	void Textbox::TextboxButton::onClick(){
		if(!textbox->isEnabled())
			textbox->enable();
		else
			textbox->disable();
	}

	Textbox::Textbox(Vector2 pos, Vector2 size, string fontPath, wstring entry){
		this->pos = pos;
		this->size = size;
		this->fontPath = fontPath;

		textboxButton = new TextboxButton(this, pos, size, "TextboxButton");
		string libPath = Root::getSingleton()->getLibPath();

		guiNode = Root::getSingleton()->getGuiNode();
		text = new Text(fontPath, entry);
		text->setScale(.5);
		Material *textMat = new Material(libPath + "text");
		textMat->addBoolUniform("texturingEnabled", false);
		textMat->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));
		text->setMaterial(textMat);
		textNode = new Node(Vector3(pos.x, pos.y + size.y, -.1));
		textNode->addText(text);
		guiNode->attachChild(textNode);

		cursorPosOffset = entry.length();
		cursorRect = new Quad(Vector3(cursorWidth, size.y, 0), false);
		cursorNode = new Node(Vector3(pos.x + text->getLength(), pos.y, cursorZCoord));
		Material *mat = new Material(libPath + "gui");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));
		cursorRect->setMaterial(mat);
		cursorNode->attachMesh(cursorRect);
		cursorNode->setVisible(false);
		guiNode->attachChild(cursorNode);
	}
	
	Textbox::~Textbox(){
		guiNode->dettachChild(textNode);
		guiNode->dettachChild(cursorNode);
		delete textNode;	
		delete cursorNode;
	}

	void Textbox::update(){
		if(enabled){
			if(canChangeCursor()){
				canShowCursor = !canShowCursor;
				lastBlinkTime = getTime();
				cursorNode->setVisible(canShowCursor);
			}
		}
	}

	void Textbox::type(u32 c, bool capitalLetters){
		entry += c;
		text->setText(entry);
		moveCursor(false, text->getGlyph(c)->size.x);
	}

	void Textbox::moveCursor(bool left, float charWidth){
		Vector3 p = cursorNode->getPosition();

		if(left && entry.size() - cursorPosOffset > 0){
			cursorPosOffset--;
			cursorNode->setPosition(Vector3(p.x - charWidth, p.y, p.z));
		}
		else if(!left){
			cursorPosOffset++;
			cursorNode->setPosition(Vector3(p.x + charWidth, p.y, p.z));
		}
	}

	void Textbox::deleteCharacter(){
		if(entry.length() > 0 && canDeleteChar()){
			char c = entry.c_str()[entry.length() - 1];
			entry = entry.substr(0, entry.length() - 1);
			text->setText(entry);
			moveCursor(true, text->getGlyph(c)->size.x);
			lastDeleteTime = getTime();
		}
	}

	void Textbox::enable(){
		enabled = true;
		lastBlinkTime = getTime();
		cursorNode->setVisible(true);
	}

	void Textbox::disable(){
		enabled = false;
		cursorNode->setVisible(false);

		if(slider){
				float val = strtof(wstringToString(entry).c_str(), nullptr);
				
				if(val < slider->getMinValue())
						val = slider->getMinValue();
				else if(val > slider->getMaxValue())
						val = slider->getMaxValue();

				slider->setValue(val);
		}
	}

	void Textbox::setEntry(wstring entry){
		this->entry = entry;
		text->setText(entry);
		cursorNode->setPosition(Vector3(pos.x + text->getLength(), pos.y, cursorZCoord));
		cursorPosOffset = entry.length();
	}
}
