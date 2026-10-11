#ifndef SLIDER_H
#define SLIDER_H

#include "vector.h"
#include "button.h"
#include "textbox.h"
// #include "listbox.h"
// #include "node.h"

namespace vb01 {
    class Node;
}

namespace vb01Gui{
    // Handles Moving GUI elements horizontally
    class Slider {
    public:
        Slider(vb01::Vector3, vb01::Vector2, double, double);
        ~Slider();
        void update();
        inline void setTextbox(Textbox *t){this->textbox=t;}
        void setGuiRectangle(vb01::Node *r);
        inline double getMinValue(){return minValue;}
        inline double getValue(){return value;}
        inline double getMaxValue(){return maxValue;}
        inline void setValue(double v){this->value=v;}
        inline vb01::Vector3 getPos(){return pos;}
        inline vb01::Vector2 getSize(){return size;}
        inline Textbox* getTextbox(){return textbox;}
        inline vb01::Node* getGuiRectangle(){return guiRectangle;}
    private:
        // This is what is pressed and dragged as the actual slider
        class MovableSliderButton : public Button {
        public:
            MovableSliderButton(Slider*, vb01::Vector3, vb01::Vector2, std::string, bool);
            void onClick();
            void update();
        private:
            Slider *slider;
            bool clicked = false;
        };

        class StaticSliderButton : public Button {
        public:
            StaticSliderButton(Slider*, vb01::Vector3, vb01::Vector2, std::string, bool);
            void onClick();
        private:
            Slider *slider;
        };

        MovableSliderButton *movableSliderButton = nullptr;
        StaticSliderButton *staticSliderButton = nullptr;
        // minValue and maxValue are the ends of the slider
        // value is where the sliders currently is between or equal to them
        double minValue, value, maxValue;
		vb01::Vector3 pos;
		vb01::Vector2 size;
        Textbox *textbox = nullptr;
        vb01::Node *guiRectangle = nullptr;
    public:
        inline MovableSliderButton* getMovableSliderButton(){return movableSliderButton;}
        inline StaticSliderButton* getStaticSliderButton(){return staticSliderButton;}
    };
}

#endif
