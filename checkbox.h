#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "button.h"

namespace vb01{
		class Quad;
		class Node;
		class Text;
}

namespace vb01Gui{
    class Checkbox {
    public:
        Checkbox(vb01::Vector2, std::string);
        ~Checkbox();
        void update(){}
        virtual void check();
        virtual bool isChecked(){return checked;}
    private:
        class CheckboxButton : public Button {
        public:
            CheckboxButton(Checkbox*, vb01::Vector2, vb01::Vector2, std::string, bool);
            void onClick();
        private:
            Checkbox *checkbox = nullptr;
        };

        const int length = 15;
        bool checked = false;
				vb01::Vector2 pos;
        CheckboxButton *checkboxButton;
				vb01::Node *checkNode = nullptr;
    public:
        inline CheckboxButton* getCheckboxButton(){return checkboxButton;}
    };
}

#endif
