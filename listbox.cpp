#include <glfw3.h>

#include "listbox.h"
#include "util.h"
#include "node.h"
#include "text.h"
#include "quad.h"
#include "root.h"
#include "material.h"

using namespace vb01;
using namespace std;

namespace vb01Gui{
	Listbox::ListboxButton::ListboxButton(Listbox *l, Vector2 pos, Vector2 size, string name) : Button(pos, size, name, "", -1, false){listbox = l;}

	void Listbox::ListboxButton::onClick(){
		if(!listbox->isOpen())
			listbox->openUp();
		else{
				Vector2 cursorPos = getCursorPos();

				if(cursorPos.x < listbox->getScrollingButton()->getPos().x){
					if(listbox->isCloseable())
						listbox->close();
				}
				else
					listbox->scrollToHeight(cursorPos.y);
		}
	}

	Listbox::ScrollingButton::ScrollingButton(Vector2 pos, Vector2 size, string name) : Button(pos, size, name, "", -1, false){}

	void Listbox::ScrollingButton::onClick(){}

	Listbox::Listbox(Vector2 pos, Vector2 size, std::vector<string> &lines, int maxDisplay, string fontPath, bool closeable){
		this->pos = pos;
		this->size = size; 
		this->maxDisplay = maxDisplay;
		this->fontPath = fontPath;
		this->lineHeight = size.y;
		this->closeable = closeable;

		Root *root = Root::getSingleton();
		Vector2 mousePos = getCursorPos();
		guiNode = Root::getSingleton()->getGuiNode();
		string texUni = "texturingEnabled", diffColUni = "diffuseColor";

		for(int i = 0; i < lines.size(); i++){
			Text *text = new Text(fontPath, stringToWstring(lines[i]));
			text->setScale(.2);

			Material *textMat = new Material(root->getLibPath() + "text");
			textMat->addBoolUniform(texUni, false);
			textMat->addVec4Uniform(diffColUni, Vector4::VEC_IJKL);
			text->setMaterial(textMat);

			Node *node = new Node(Vector3(pos.x, pos.y + size.y * (i + 1), textZCoord));
			node->addText(text);
			guiNode->attachChild(node);
			node->setVisible(false);

			this->lines.push_back(text);
		}

		this->lines[0]->getNode()->setVisible(true);

		listboxButton = new ListboxButton(this, pos, size, "ListboxButton");

		scrollingButton = new ScrollingButton(Vector2(pos.x + size.x - 20, pos.y + lineHeight), Vector2(20, lineHeight * (maxDisplay - 2) / (maxDisplay + 1)), "scrollingButton");
		scrollingButton->setColor(Vector4(.2, .2, .2, 1));
		scrollingButton->setZOrder(-.05);
		scrollingButton->setActive(false);

		Quad *selRect = new Quad(Vector3(size.x, size.y, 0), false);
		selRectNode = new Node(Vector3(size.x, size.y, -.05));
		Material *mat = new Material(root->getLibPath() + "gui");
		mat->addBoolUniform(texUni, false);
		mat->addVec4Uniform(diffColUni, Vector4(.6, .35, .05, 1));
		selRect->setMaterial(mat);
		selRectNode->attachMesh(selRect);
		selRectNode->setVisible(false);
		guiNode->attachChild(selRectNode);

		if(!closeable)
			openUp();
	}

	Listbox::~Listbox(){
		while(!lines.empty()){
			int id = lines.size() - 1;
			Node *textNode = lines[id]->getNode();
			guiNode->dettachChild(textNode);
			delete textNode;
			lines.pop_back();
		}

		guiNode->dettachChild(selRectNode);
		delete selRectNode;
	}

	void Listbox::update(){
		scrollingButton->update();
		listboxButton->update();

		if(open){
			Vector2 mousePos = getCursorPos();
			Vector2 p = pos;

			if(mousePos.y > pos.y && mousePos.y < pos.y + size.y * maxDisplay){
				selectedOption = scrollOffset + ((int)(mousePos.y - pos.y) / ((int)size.y));
				p.y = pos.y + size.y * ((int)(mousePos.y - pos.y) / ((int)size.y));
			}
			else if(mousePos.y < pos.y){
				selectedOption = 0;
				p.y = pos.y;
			}
			else{
				selectedOption = (maxDisplay + scrollOffset) - 1;
				p.y = pos.y + size.y * (maxDisplay - 1);
			}

			selRectNode->setPosition(Vector3(p.x, p.y, -.05));

			Vector2 scrollButtonPos = Vector2(pos.x + size.x - 20, pos.y + lineHeight + lineHeight * scrollOffset * (double(maxDisplay - 2) / (maxDisplay + 1)));
			scrollingButton->setPos(scrollButtonPos);
		} 
	}

	void Listbox::scrollToHeight(float clickPos){
		float clickHeight = clickPos - (pos.y + lineHeight);
		int newOffset = clickHeight / (lineHeight * (maxDisplay - 2) / (maxDisplay + 1));
		int diffOffset = scrollOffset - newOffset;

		for(int i = 0; i < abs(diffOffset); ++i){
				if(diffOffset > 0)
					scrollUp();
				else
					scrollDown();
		}
	}

	void Listbox::openUp(){
		open = true;
		lines[selectedOption]->getNode()->setPosition(Vector3(pos.x, pos.y + size.y * (selectedOption-scrollOffset + 1), textZCoord));
		lines[selectedOption]->getNode()->setVisible(false);
		Vector2 size = listboxButton->getSize();
		size.y *= maxDisplay;
		listboxButton->setSize(size);
		selRectNode->setVisible(true);
		scrollingButton->setActive(true);

		for(int i = scrollOffset; i < scrollOffset + maxDisplay; i++)
			lines[i]->getNode()->setVisible(true);

		onOpen();
	}

	void Listbox::close(){
		open = false;
		Vector2 size = listboxButton->getSize();
		size.y /= maxDisplay;
		listboxButton->setSize(size);
		selRectNode->setVisible(false);
		scrollingButton->setActive(false);

		for(int i = scrollOffset; i < scrollOffset + maxDisplay; i++)
			lines[i]->getNode()->setVisible(false);

		lines[selectedOption]->getNode()->setPosition(Vector3(pos.x, pos.y + size.y, textZCoord));
		lines[selectedOption]->getNode()->setVisible(true);

		onClose();
	}

	void Listbox::scrollUp(){
		if(scrollOffset > 0){
			scrollOffset--;

			for(Text *l : lines){
				Node *node = l->getNode();
				Vector3 p = node->getPosition();
				node->setPosition(Vector3(p.x, p.y + size.y, p.z));

				if(node->getPosition().y > pos.y + size.y * maxDisplay)
					node->setVisible(false);
				else if(node->getPosition().y > pos.y)
					node->setVisible(true);
			}
		}
	}

	void Listbox::scrollDown(){
		if (scrollOffset < lines.size() - maxDisplay){
			scrollOffset++;

			for(Text *l : lines){
				Node *node = l->getNode();
				Vector3 p = node->getPosition();
				node->setPosition(Vector3(p.x, p.y - size.y, p.z));

				if(node->getPosition().y - size.y < pos.y)
					node->setVisible(false);
				else if(node->getPosition().y < pos.y + size.y * (maxDisplay + 1))
					node->setVisible(true);
			}
		}
	}

	void Listbox::changeLine(int id, wstring change){
		lines[id]->setText(change);	
	}

	std::vector<wstring> Listbox::getContents(){
		std::vector<wstring> lines;

		for(Text *t : this->lines)
			lines.push_back(t->getText());

		return lines;
	}

	void Listbox::addLine(wstring line){
		Text *t = new Text(fontPath, line);
		Node *node = new Node(Vector3(pos.x, pos.y - size.y * lines.size(), textZCoord));
		node->addText(t);
		guiNode->attachChild(node);
		lines.push_back(t);
	}

	string Listbox::convert(string str){
		return str;
	}
}
