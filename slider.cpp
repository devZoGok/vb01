#include <glfw3.h>

#include <sstream>
#include <algorithm>

#include "slider.h"
#include "root.h"
#include "util.h"


using namespace std;
using namespace vb01;

namespace vb01Gui{
    Slider::MovableSliderButton::MovableSliderButton(Slider *slider, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, "", -1, separate) {
        this->slider = slider;
    }

    void Slider::MovableSliderButton::onClick() {
        clicked = !clicked;
    }

    void Slider::MovableSliderButton::update() {
				Vector2 cursorPos;

        if (clicked && cursorPos.y > slider->getPos().y && cursorPos.y < slider->getPos().y + slider->getSize().y) {
            if (cursorPos.x < slider->getPos().x)
                cursorPos.x = slider->getPos().x;
            else if (cursorPos.x > slider->getPos().x + slider->getSize().x)
                cursorPos.x = slider->getPos().x + slider->getSize().x;

            slider->setValue((((double) cursorPos.x - slider->getPos().x) / slider->getSize().x) * slider->getMaxValue());
        }
    }

    Slider::StaticSliderButton::StaticSliderButton(Slider *slider, Vector2 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, "", -1, separate) {
        this->slider = slider;
    }

    void Slider::StaticSliderButton::onClick() {
				Vector2 cursorPos = getCursorPos();
        s16 buttonClickPoint = -slider->getPos().x + cursorPos.x;
        slider->setValue(((double) buttonClickPoint / slider->getSize().x) * slider->getMaxValue());
    }

    Slider::Slider(Vector2 pos, Vector2 size, double minValue, double maxValue) {
        this->pos = pos;
        this->size = size;
        this->minValue = minValue;
        this->maxValue = maxValue;

        staticSliderButton = new StaticSliderButton(this, pos, size, "StaticSliderButton", false);
        movableSliderButton = new MovableSliderButton(this, Vector2(pos.x + staticSliderButton->getSize().x / 1, pos.y - 15), Vector2(10, 40), "MovableSliderButton", false);

        value = (maxValue - minValue) / 2;
    }

    Slider::~Slider() {
    }

    void Slider::update() {
        if (value < minValue)
            value = minValue;
        else if (value > maxValue)
            value = maxValue;

        Vector2 p = movableSliderButton->getPos(), s = movableSliderButton->getSize();
        p.x = pos.x + (double) size.x / maxValue * value;
        movableSliderButton->update();
        movableSliderButton->setPos(p);

				if(textbox && !textbox->isEnabled()){
						ostringstream ss;
						ss << value;
						textbox->setEntry(stringToWstring(ss.str()));
				}
    }
}
