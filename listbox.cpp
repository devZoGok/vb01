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
	Listbox::ListboxButton::ListboxButton(Listbox *l, Vector3 pos, Vector2 size, string name) : Button(pos, size, name, "", -1, false), listbox(l) {}

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

	Listbox::ScrollingButton::ScrollingButton(Vector3 pos, Vector2 size, string name) : Button(pos, size, name, "", -1, false){}

	void Listbox::ScrollingButton::onClick(){}

	Listbox::Listbox(Vector3 p, Vector2 s, std::vector<string> &lines, int md, string fp, bool cl) : 
		pos(p),
		size(s),
		maxDisplay(md),
		fontPath(fp),
		closeable(cl),
		textOffset(Vector3(0, 0, .3))
	{
		Root *root = Root::getSingleton();
		Vector2 mousePos = getCursorPos();
		guiNode = Root::getSingleton()->getGuiNode();

		string texUni = "texturingEnabled", diffColUni = "diffuseColor";
		textMat = new Material(root->getLibPath() + "text");
		textMat->addBoolUniform(texUni, false);
		textMat->addVec4Uniform(diffColUni, Vector4::VEC_IJKL);

		if(!lines.empty()){
			for(int i = 0; i < lines.size(); i++)
				addLine(stringToWstring(lines[i]));

			this->lines[0]->getNode()->setVisible(true);
		}

		listboxButton = new ListboxButton(this, pos, size, "ListboxButton");

		scrollingButton = new ScrollingButton(Vector3(pos.x + size.x - 20, pos.y + lineHeight, pos.z - .05), Vector2(20, lineHeight * (maxDisplay - 2) / (maxDisplay + 1)), "scrollingButton");
		scrollingButton->setColor(Vector4(.2, .2, .2, 1));
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

	//TODO selection rectangle Z offset
	void Listbox::update(){
		scrollingButton->update();
		listboxButton->update();

		if(open){
			Vector2 mousePos = getCursorPos();
			Vector3 p = pos;

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

			selRectNode->setPosition(Vector3(p.x, p.y, p.z + .05));

			Vector3 scrollButtonPos = Vector3(pos.x + size.x - 20, pos.y + lineHeight + lineHeight * scrollOffset * (double(maxDisplay - 2) / (maxDisplay + 1)), pos.z - .05);
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
		if(lines.empty()) return;

		open = true;
		Vector3 selOpPos = pos + Vector3(0, size.y * (selectedOption - scrollOffset + 1), 0) + textOffset;
		lines[selectedOption]->getNode()->setPosition(selOpPos);
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

		lines[selectedOption]->getNode()->setPosition(pos + Vector3(0, size.y, 0) + textOffset);
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
		float sc = .2;
		Node *node = new Node(pos + Vector3(0, size.y * (lines.size() + 1), 0) + textOffset, Quaternion::QUAT_W, Vector3(sc, sc, 1));
		Root::getSingleton()->getGuiNode()->attachChild(node);
		node->setVisible(false);

		Text *text = new Text(fontPath, line);
		text->setMaterial(textMat);
		node->addText(text);
		
		lines.push_back(text);
	}

	string Listbox::convert(string str){
		return str;
	}
}
