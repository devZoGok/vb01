#include <glfw3.h>

#include <sstream>
#include <algorithm>

#include "slider.h"
#include "root.h"
#include "util.h"
#include "node.h"


using namespace std;
using namespace vb01;

namespace vb01Gui{
    // The variables passed to the Button object mean that the movable slider's trigger input is -1, the imagePath is left empty
    // Slider is initialized as s1
    Slider::MovableSliderButton::MovableSliderButton(Slider *sl, Vector3 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, "", -1, separate), slider(sl) {}

    void Slider::MovableSliderButton::onClick() {
        clicked = !clicked;
    }

    /// @brief Updates the slider value that is used for the slider position
    void Slider::MovableSliderButton::update() {
		Vector2 cursorPos = getCursorPos();

        if (clicked && cursorPos.y > slider->getPos().y && cursorPos.y < slider->getPos().y + slider->getSize().y) {
            if (cursorPos.x < slider->getPos().x)
                cursorPos.x = slider->getPos().x;
            else if (cursorPos.x > slider->getPos().x + slider->getSize().x)
                cursorPos.x = slider->getPos().x + slider->getSize().x;

            slider->setValue((((double) cursorPos.x - slider->getPos().x) / slider->getSize().x) * slider->getMaxValue());
        }
    }

    Slider::StaticSliderButton::StaticSliderButton(Slider *sl, Vector3 pos, Vector2 size, string name, bool separate) : Button(pos, size, name, "", -1, separate), slider(sl) {}

    /// @brief Wherever the static slider button is clicked, the slider will be set to
    void Slider::StaticSliderButton::onClick() {
		Vector2 cursorPos = getCursorPos();
        s16 buttonClickPoint = -slider->getPos().x + cursorPos.x;
        slider->setValue(((double) buttonClickPoint / slider->getSize().x) * slider->getMaxValue());
    }

    Slider::Slider(Vector3 p, Vector2 s, double min, double max) : pos(p), size(s), minValue(min), maxValue(max) {
        staticSliderButton = new StaticSliderButton(this, pos, size, "StaticSliderButton", false);
        movableSliderButton = new MovableSliderButton(this, pos + Vector3(staticSliderButton->getSize().x, -15, 0), Vector2(10, 40), "MovableSliderButton", false);
        
        // The slider begins in the center 
        value = (maxValue - minValue) / 2;
    }

    Slider::~Slider() {}

    /// @brief Updates the movable slider button based on the new value (the static slider button is not updated since it is updted when clicked)
    void Slider::update() {
        //  Clamps the value to the slider min and max range
        if (value < minValue)
            value = minValue;
        else if (value > maxValue)
            value = maxValue;

        Vector3 p = movableSliderButton->getPos(); 
		// Vector2 s = movableSliderButton->getSize();
        // This sets the position of the slider
        float sliderPos = pos.x + (double) size.x / maxValue * value;
        // This applies that position to the movable slider button
        p.x = sliderPos;
        // Update the slider value to be ready for the next frame
        movableSliderButton->update();
        movableSliderButton->setPos(p);

         // If this slider has a guiRectangle that is visible
        if(guiRectangle && guiRectangle->isVisible())
        {
            // Get the current position of the gui rectangle and then update it propotionally as the slider position change (only in the x direction)
           Vector3 currentRecP = guiRectangle->getPosition(); 
           // Get the size of the gui rectangle for the calculation
           //  Vector3 currentRecS = guiRectangle->getScale();

           // Set the gui rectangle to the same position as the slider
            currentRecP.x = sliderPos;
           // Sets the position of the GUI rectangle based on the proportional distance the slider moved
            guiRectangle->setPosition(currentRecP);
        }


        // If this slider has a textbox but it is not enabled then it sets the textbox's text to the typed in string 
		if(textbox && !textbox->isEnabled()){
			ostringstream ss;
			ss << value;
			textbox->setEntry(stringToWstring(ss.str()));
		}
    }

    void Slider::setGuiRectangle(Node *r)
    {
        this->guiRectangle = r;

        // Get scale of the gui and set the position to the middle of the scale x axis
        // Vector3 RecScale = guiRectangle->getScale();
        Vector3 RecPos = guiRectangle->getPosition();
        // Set the position of the gui rectangle to the same as the slider
        RecPos.x = value;
        // RecPos.x = RecScale.x / 2;

        guiRectangle->setPosition(RecPos);
    }
}
