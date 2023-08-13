#ifndef SLIDER_H
#define SLIDER_H

#include "vector.h"
#include "button.h"
#include "textbox.h"

namespace vb01Gui{
    class Slider {
    public:
        Slider(vb01::Vector2, vb01::Vector2, double, double);
        ~Slider();
        void update();
        inline void setTextbox(Textbox *t){this->textbox=t;}
        inline double getMinValue(){return minValue;}
        inline double getValue(){return value;}
        inline double getMaxValue(){return maxValue;}
        inline void setValue(double v){this->value=v;}
        inline vb01::Vector2 getPos(){return pos;}
        inline vb01::Vector2 getSize(){return size;}
        inline Textbox* getTextbox(){return textbox;}
    private:
        class MovableSliderButton : public Button {
        public:
            MovableSliderButton(Slider*, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
            void update();
        private:
            Slider *slider;
            bool clicked = false;
        };

        class StaticSliderButton : public Button {
        public:
            StaticSliderButton(Slider*, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        private:
            Slider *slider;
        };

        MovableSliderButton *movableSliderButton = nullptr;
        StaticSliderButton *staticSliderButton = nullptr;
        double minValue, value, maxValue;
				vb01::Vector2 pos, size;
        Textbox *textbox = nullptr;
    public:
        inline MovableSliderButton* getMovableSliderButton(){return movableSliderButton;}
        inline StaticSliderButton* getStaticSliderButton(){return staticSliderButton;}
    };
}

#endif
