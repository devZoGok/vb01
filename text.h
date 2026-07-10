#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

#include "fontAsset.h"
#include "attachable.h"
#include "vector.h"
#include "util.h"

namespace vb01{
	class Node;
	class Shader;
	class Material;

	class Text : public Attachable{
		public:
			struct Character{
				wchar_t ch;
				Material *material = nullptr;
				Vector2 offset = Vector2::VEC_ZERO;

				Character(wchar_t c, Material *m) : ch(c), material(m) {}
			};

			Text(std::string, std::wstring, u16 = 0, u16 = 256);
			~Text();
			void update();
			std::wstring getEntry();
			void setEntry(std::wstring);
			inline FontAsset* getFont(){return font;}
			inline int getLength(){return characters.size();}
			inline bool isHorizontal(){return horizontal;}
			inline void setHorizontal(bool h){this->horizontal = h;}
			inline bool isLeftToRight(){return leftToRight;}
			inline void setLeftToRight(bool ltr){this->leftToRight = ltr;}
			inline Material* getMaterial(){return material;}
			inline void setMaterial(Material *mat){this->material = mat;}
			inline const std::vector<Character>& getCharacters(){return characters;}
			//Glyph* getGlyph(u16);
		private:
			void init();
			void applyFont(std::string, u16, u16 = 256);
			void clearFont();
			//void prepareGlyphs(Glyph, int, Vector2);

			FontAsset *font = nullptr;
			Material *material = nullptr;
			std::vector<Character> characters;
			bool horizontal = true, leftToRight = true;
	};
}

#endif
