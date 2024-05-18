#ifndef LISTBOX_H
#define LISTBOX_H

#include <vector>

#include "button.h"
#include "vector.h"

namespace vb01{
	class Node;
	class Text;
}

namespace vb01Gui{
	class Listbox{
		public: 
			class ListboxButton : public Button{
				public:
					ListboxButton(Listbox*, vb01::Vector3, vb01::Vector2, std::string);
					void onClick();
				private:
					Listbox *listbox = nullptr;
			};

			Listbox(vb01::Vector3, vb01::Vector2, std::vector<std::string>&, int, std::string, bool = true);
			~Listbox();
			void update();
			void openUp();
			void close();
			void scrollUp();
			void scrollDown();
			void addLine(std::wstring);
			void changeLine(int, std::wstring);
			void scrollToHeight(float);
			virtual void onOpen(){}
			virtual void onClose(){}
			inline int getScrollOffset(){return scrollOffset;}
			inline float getLineHeight(){return lineHeight;}
			inline bool isOpen(){return open;}
			inline bool isCloseable(){return closeable;}
			void appendLines(std::vector<std::wstring>&);
			inline int getSelectedOption(){return selectedOption;}
			inline int getMaxDisplay(){return maxDisplay;}
			inline int getNumLines(){return lines.size();}
			std::vector<std::wstring> getContents();
			inline vb01::Vector3 getPos(){return pos;} 
			inline vb01::Vector2 getSize(){return size;} 
		private: 
			class ScrollingButton : public Button{
				public:
					ScrollingButton(vb01::Vector3, vb01::Vector2, std::string);
					void onClick();
				private:
			};

			std::string convert(std::string);

			float lineHeight;
			int maxDisplay, scrollOffset = 0;
			bool open = false, closeable = false;
			std::string fontPath;
			vb01::Vector3 pos, textOffset;
			vb01::Vector2 size;
			ListboxButton *listboxButton;
			ScrollingButton *scrollingButton;
			vb01::Node *selRectNode, *guiNode;
			std::vector<vb01::Text*> lines;
		protected:
		   	int selectedOption = 0;
		public: 
			inline ListboxButton* getListboxButton(){return listboxButton;}
			inline ScrollingButton* getScrollingButton(){return scrollingButton;}
	};
}

#endif
