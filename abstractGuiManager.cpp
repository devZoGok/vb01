#include <algorithm>

#include "abstractGuiManager.h"
#include "textbox.h"
#include "slider.h"
#include "checkbox.h"
#include "listbox.h"
#include "util.h"

namespace vb01Gui{
	using namespace std;
	using namespace vb01;

    void AbstractGuiManager::update() {
        Vector2 cursorPos = getCursorPos();

        for (Button *b : buttons) {
            if (b->isSeparate())
                b->update();

            bool withinX = (cursorPos.x > b->getPos().x && cursorPos.x < b->getPos().x + b->getSize().x);
            bool withinY = (cursorPos.y > b->getPos().y && cursorPos.y < b->getPos().y + b->getSize().y);

            if(withinX && withinY)
                b->onMouseOver();
            else
                b->onMouseOff();
        }

        for (Listbox *l : listboxes)
            l->update();

        for (Checkbox *c : checkboxes)
            c->update();

        for (Slider *s : sliders)
            s->update();

		if(currentSlider){
			float percentage = (cursorPos.x - currentSlider->getPos().x) / currentSlider->getSize().x;
			currentSlider->setValue(percentage * currentSlider->getMaxValue());
		}

        for (Textbox *t : textboxes){
            t->update();

		}
    }

	void AbstractGuiManager::updateCurrentListbox(Button *b, bool &listboxClicked){
		Listbox *pastListbox = currentListbox;
		
		for(Listbox *l : listboxes){
			if(b == l->getListboxButton() || b == l->getScrollingButton()){
				currentListbox = l;
				listboxClicked = true;
		
				if(pastListbox && currentListbox != pastListbox){
					if(pastListbox->isCloseable())
						pastListbox->close();
				}
				else
					currentListbox = (l->isOpen() ? l : nullptr);
		
				break;
			}
		}
	}

	void AbstractGuiManager::updateCurrentSlider(Button *b){
		for(Slider *s : sliders)
			if(b == s->getMovableSliderButton()){
				currentSlider = s;
				break;
			}
	}

	void AbstractGuiManager::updateCurrentTextbox(Button *b, bool &textboxClicked){
		Textbox *pastTextbox = currentTextbox;

		for(Textbox *t : textboxes)
			if(b == t->getTextboxButton()){
				currentTextbox = t;
				textboxClicked = true;
		
				if(pastTextbox && currentTextbox != pastTextbox)
					pastTextbox->disable();
				else
					currentTextbox = (t->isEnabled() ? t : nullptr);
		
				break;
			}
	}

	void AbstractGuiManager::findClickedButton(){
		bool textboxClicked = false, listboxClicked = false;
		
		for (int i = 0; i < buttons.size(); i++) {
			Vector2 mousePos = getCursorPos();
			Button *b = buttons[i];
			bool withinX = mousePos.x > b->getPos().x && mousePos.x < b->getPos().x + b->getSize().x;
			bool withinY = mousePos.y > b->getPos().y && mousePos.y < b->getPos().y + b->getSize().y;
		
			if (b->isActive() && withinX && withinY){
			    b->onClick();
				updateCurrentListbox(b, listboxClicked);
				updateCurrentSlider(b);
				updateCurrentTextbox(b, textboxClicked);
			}
		}
		
		if(!textboxClicked && currentTextbox){
			currentTextbox->disable();
			currentTextbox = nullptr;
		}
		
		if(!listboxClicked && currentListbox && currentListbox->isCloseable()){
			currentListbox->close();
			currentListbox = nullptr;
		}
	}

    void AbstractGuiManager::addButton(Button* b) {
        buttons.push_back(b);
    }

    void AbstractGuiManager::addListbox(Listbox* l) {
        listboxes.push_back(l);
        buttons.push_back(l->getListboxButton());
        buttons.push_back(l->getScrollingButton());
    }

    void AbstractGuiManager::addCheckbox(Checkbox* c) {
        buttons.push_back(c->getCheckboxButton());
        checkboxes.push_back(c);
    }

    void AbstractGuiManager::addSlider(Slider* s) {
        buttons.push_back(s->getMovableSliderButton());
        buttons.push_back(s->getStaticSliderButton());
        sliders.push_back(s);
    }

    void AbstractGuiManager::addTextbox(Textbox* t) {
        buttons.push_back(t->getTextboxButton());
        textboxes.push_back(t);
    }

    void AbstractGuiManager::removeButton(Button *b) {
        for (int i = 0; i < buttons.size(); i++) {
            if (b == buttons[i]) {
                delete b;
                buttons.erase(buttons.begin() + i);
            }
        }
    }

    void AbstractGuiManager::removeButton(string name) {
        for (int i = 0; i < buttons.size(); i++) {
            if (name == buttons[i]->getName() && buttons[i]->isSeparate()) {
                delete buttons[i];
                buttons.erase(buttons.begin() + i);
            }
        }
    }

    void AbstractGuiManager::removeListbox(Listbox *l) {
        for (int i = 0; i < listboxes.size(); i++) {
            if (l == listboxes[i]) {
                removeButton(l->getListboxButton());
                delete l;
                listboxes.erase(listboxes.begin() + i);
            }
        }
    }

    void AbstractGuiManager::removeCheckbox(Checkbox *c) {
        for (int i = 0; i < checkboxes.size(); i++) {
            if (c == checkboxes[i]) {
                removeButton(c->getCheckboxButton());
                delete c;
                checkboxes.erase(checkboxes.begin() + i);
            }
        }
    }

    void AbstractGuiManager::removeSlider(Slider *s) {
        for (int i = 0; i < sliders.size(); i++) {
            if (s == sliders[i]) {
                removeButton(s->getMovableSliderButton());
                removeButton(s->getStaticSliderButton());
                delete s;
                sliders.erase(sliders.begin() + i);
            }
        }
    }

    void AbstractGuiManager::removeTextbox(Textbox* t) {
        for (int i = 0; i < textboxes.size(); i++) {
            if (t == textboxes[i]) {
                removeButton(t->getTextboxButton());
                delete t;
                textboxes.erase(textboxes.begin() + i);
            }
        }
    }

	void AbstractGuiManager::removeAllButtons(vector<Button*> exceptions){
		int targetId = 0;
		
		while(targetId != buttons.size()){
			if(buttons[targetId]->isSeparate() && find(exceptions.begin(), exceptions.end(), buttons[targetId]) == exceptions.end()){
				delete buttons[targetId];
				buttons.erase(buttons.begin() + targetId);
			}
			else
				targetId++;
		}
	}

    void AbstractGuiManager::removeAllListboxes(vector<Listbox*> exceptions) {
		int targetId = 0;

        while(targetId != listboxes.size()) {
			if(find(exceptions.begin(), exceptions.end(), listboxes[targetId]) == exceptions.end()){
				removeButton(listboxes[listboxes.size() - 1]->getListboxButton());
				delete listboxes[listboxes.size() - 1];
				listboxes.pop_back();
			}
			else
				targetId++;
        }
    }

    void AbstractGuiManager::removeAllCheckboxes(vector<Checkbox*> exceptions) {
		int targetId = 0;

        while(targetId != checkboxes.size()) {
			if(find(exceptions.begin(), exceptions.end(), checkboxes[targetId]) == exceptions.end()){
			   removeButton(checkboxes[checkboxes.size() - 1]->getCheckboxButton());
			   delete checkboxes[checkboxes.size() - 1];
			   checkboxes.pop_back();
			}
			else
				targetId++;
        }
    }

    void AbstractGuiManager::removeAllSliders(vector<Slider*> exceptions) {
		int targetId = 0;

        while(targetId != sliders.size()) {
			if(find(exceptions.begin(), exceptions.end(), sliders[targetId]) == exceptions.end()){
            	removeButton(sliders[sliders.size() - 1]->getMovableSliderButton());
            	removeButton(sliders[sliders.size() - 1]->getStaticSliderButton());
            	delete sliders[sliders.size() - 1];
            	sliders.pop_back();
			}
			else
				targetId++;
        }
    }

    void AbstractGuiManager::removeAllTextboxes(vector<Textbox*> exceptions) {
		int targetId = 0;

        while(targetId != textboxes.size()) {
			if(find(exceptions.begin(), exceptions.end(), textboxes[targetId]) == exceptions.end()){
				removeButton(textboxes[textboxes.size() - 1]->getTextboxButton());
				delete textboxes[textboxes.size() - 1];
				textboxes.pop_back();
			}
			else
				targetId++;
        }
    }

	void AbstractGuiManager::removeAllGuiElements(
			vector<Button*> buttonExceptions,
			vector<Listbox*> listboxExceptions,
			vector<Checkbox*> checkboxExceptions,
			vector<Slider*> sliderExceptions,
			vector<Textbox*> textboxExceptions){
		removeAllButtons(buttonExceptions);
		removeAllListboxes(listboxExceptions);
		removeAllCheckboxes(checkboxExceptions);
		removeAllSliders(sliderExceptions);
		removeAllTextboxes(textboxExceptions);
	}
}