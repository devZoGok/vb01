#include "checkbox.h"
#include "text.h"
#include "node.h"
#include "material.h"

using namespace vb01;
using namespace std;

namespace vb01Gui{
    Checkbox::CheckboxButton::CheckboxButton(Checkbox *ch, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, "", -1, separate) {
        checkbox = ch;
    }
    
    void Checkbox::CheckboxButton::onClick() {
        checkbox->check();
				float col = (checkbox->isChecked() ? .6 : .2);
				setColor(Vector4(col, col, col, 1));
    }

    Checkbox::Checkbox(Vector2 pos, string fontPath){
        this->pos = pos;
        checkboxButton = new CheckboxButton(this, pos, Vector2(length,length), "CheckboxButton", false);

				/*
				Root *root = Root::getSingleton();
				checkNode = new Node();
				Text *check = new Text(fontPath, L"X");
				Material *mat = new Material(root->getLibPath() + "text");
				mat->addBoolUniform("texturingEnabled", false);
				mat->addVec4Uniform("diffuseColor", Vector4::VEC_IJKL);
				check->setMaterial(mat);
				checkNode->addText(check);
				checkNode->setVisible(false);
				root->getGuiNode()->attachChild(checkNode);
				checkNode->setPosition(Vector3(pos.x, pos.y, 0.1f));
				*/
    }
    
    Checkbox::~Checkbox() {

    }
    
    void Checkbox::check() {
			checked = !checked;
			//checkNode->setVisible(checked);
    }
}
