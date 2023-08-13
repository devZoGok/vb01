#ifndef ABSTRACT_GUI_MANAGER_H
#define ABSTRACT_GUI_MANAGER_H

#include <vector>
#include <string>

namespace vb01Gui{
	class Button;
	class Listbox;
	class Checkbox;
	class Slider;
	class Textbox;

	class AbstractGuiManager{
		public:
			void update();
			void findClickedButton();
			void addButton(vb01Gui::Button*);
			void removeButton(vb01Gui::Button*);
			void removeButton(std::string);
			void addListbox(vb01Gui::Listbox*);
			void removeListbox(vb01Gui::Listbox*);
			void addCheckbox(vb01Gui::Checkbox*);
			void removeCheckbox(vb01Gui::Checkbox*);
			void addTextbox(vb01Gui::Textbox*);
			void removeTextbox(vb01Gui::Textbox*);
			void addSlider(vb01Gui::Slider*);
			void removeSlider(vb01Gui::Slider*);
			void removeAllButtons(std::vector<vb01Gui::Button*> = std::vector<vb01Gui::Button*>{});
			void removeAllListboxes();
			void removeAllCheckboxes();
			void removeAllSliders();
			void removeAllTextboxes();
		private:
			void updateCurrentListbox(vb01Gui::Button*, bool&);
			void updateCurrentSlider(vb01Gui::Button*);
			void updateCurrentTextbox(vb01Gui::Button*, bool&);

			std::vector<Button*> buttons;
			std::vector<Listbox*> listboxes;
			std::vector<Checkbox*> checkboxes;
			std::vector<Slider*> sliders;
			std::vector<Textbox*> textboxes;
			vb01Gui::Slider *currentSlider = nullptr;
			vb01Gui::Textbox *currentTextbox = nullptr;
			vb01Gui::Listbox *currentListbox = nullptr;
		protected:
			AbstractGuiManager(){}
	};
}

#endif
