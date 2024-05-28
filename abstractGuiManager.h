#ifndef ABSTRACT_GUI_MANAGER_H
#define ABSTRACT_GUI_MANAGER_H

#include <vector>
#include <string>

namespace vb01{
	class Node;
	class Text;
}

namespace vb01Gui{
	class Button;
	class Listbox;
	class Checkbox;
	class Slider;
	class Textbox;

	class AbstractGuiManager{
		public:
			void update();
			std::vector<Button*> findClickedButtons();
			void updateGui();
			void addButton(Button *b){buttons.push_back(b);}
			void removeButton(Button*);
			void removeButton(std::string);
			void addListbox(Listbox*);
			void removeListbox(Listbox*);
			void addCheckbox(Checkbox*);
			void removeCheckbox(Checkbox*);
			void addTextbox(Textbox*);
			void removeTextbox(Textbox*);
			void addSlider(Slider*);
			void addGuiRectangle(vb01::Node *r){guiRectangles.push_back(r);}
			void removeGuiRectangle(vb01::Node*);
			void addText(vb01::Text *t){texts.push_back(t);}
			void removeText(vb01::Text*);
			vb01::Text* getText(std::string);
			void removeSlider(Slider*);
			void removeAllButtons(std::vector<Button*> = std::vector<Button*>{});
			void removeAllListboxes(std::vector<Listbox*> = std::vector<Listbox*>{});
			void removeAllCheckboxes(std::vector<Checkbox*> = std::vector<Checkbox*>{});
			void removeAllSliders(std::vector<Slider*> = std::vector<Slider*>{});
			void removeAllTextboxes(std::vector<Textbox*> = std::vector<Textbox*>{});
			void removeAllGuiRectangles(std::vector<vb01::Node*> = std::vector<vb01::Node*>{});
			void removeAllTexts(std::vector<vb01::Text*> = std::vector<vb01::Text*>{});
			void removeAllGuiElements(
					std::vector<Button*> = std::vector<Button*>{},
					std::vector<Listbox*> = std::vector<Listbox*>{},
					std::vector<Checkbox*> = std::vector<Checkbox*>{},
					std::vector<Slider*> = std::vector<Slider*>{},
					std::vector<Textbox*> = std::vector<Textbox*>{},
					std::vector<vb01::Node*> = std::vector<vb01::Node*>{},
					std::vector<vb01::Text*> = std::vector<vb01::Text*>{}
			);
			inline std::vector<Button*> getButtons(){return buttons;}
			inline std::vector<Listbox*> getListboxes(){return listboxes;}
			inline std::vector<Checkbox*> getCheckboxes(){return checkboxes;}
			inline std::vector<Slider*> getSliders(){return sliders;}
			inline std::vector<Textbox*> getTextboxes(){return textboxes;}
			inline std::vector<vb01::Node*> getGuiRectangles(){return guiRectangles;}
			inline std::vector<vb01::Text*> getTexts(){return texts;}
		private:
			void updateCurrentListbox(Button*, bool&);
			void updateCurrentSlider(Button*);
			void updateCurrentTextbox(Button*, bool&);

			std::vector<Button*> buttons;
			std::vector<Listbox*> listboxes;
			std::vector<Checkbox*> checkboxes;
			std::vector<Slider*> sliders;
			std::vector<Textbox*> textboxes;
			std::vector<vb01::Node*> guiRectangles;
			std::vector<vb01::Text*> texts;
			Slider *currentSlider = nullptr;
			Textbox *currentTextbox = nullptr;
			Listbox *currentListbox = nullptr;
		protected:
			AbstractGuiManager(){}
	};
}

#endif
