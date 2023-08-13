#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "util.h"
#include "text.h"
#include "button.h"

namespace vb01{
	class Node;
	class Quad;
}

namespace vb01Gui{
	class Slider;

	class Textbox{
		public: 
			Textbox(vb01::Vector2, vb01::Vector2, std::string, std::wstring = L"");
			~Textbox();
			void update();
			void enable();
			void disable();
			void type(vb01::u32, bool = false);
			void moveCursor(bool, float);
			void deleteCharacter();
			void setEntry(std::wstring);
			void remove();
			inline void setSlider(Slider *s){this->slider = s;}
			inline bool isEnabled(){return enabled;}
			inline std::wstring getText(){return text->getText();}
			inline void setDeleteCharacters(bool del){this->deleteCharacters = del;}
			inline bool isDeleteCharacters(){return deleteCharacters;}
		private:
			class TextboxButton : public Button{
				public:
					TextboxButton(Textbox*, vb01::Vector2, vb01::Vector2, std::string);
					void onClick();
				private:
					Textbox *textbox;
			};
			inline bool canChangeCursor(){return vb01::getTime() - lastBlinkTime > 250;}
			inline bool canDeleteChar(){return vb01::getTime() - lastDeleteTime > 50;}

			float cursorZCoord = -.2;
			const int cursorWidth = 5;
			vb01::Vector2 pos, size;
			std::wstring entry = L"";
			std::string fontPath = "";
			bool enabled = false, canShowCursor = false, capitalLeters = false, deleteCharacters = false;
			vb01::s64 lastBlinkTime = 0, lastDeleteTime = 0, cursorPosOffset = 0;
			vb01::Quad *cursorRect;
			vb01::Text *text;
			vb01::Node *guiNode, *textNode, *cursorNode;
			TextboxButton *textboxButton;
			Slider *slider = nullptr;
		public:
			inline TextboxButton* getTextboxButton(){return textboxButton;}
	};
}

#endif
