#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

#include "attachable.h"
#include "vector.h"
#include "util.h"

namespace vb01{
	class Node;
	class Texture;
	class Shader;
	class Material;

	class Text : public Attachable{
		public:
			struct Glyph{
				u16 ch;
				Texture *texture = nullptr;
				u32 advance;
				Vector2 size, bearing;
			};

			Text(std::string, std::wstring, u16 = 0, u16 = 256);
			~Text();
			void update();
			void setText(std::wstring);
			inline void setScale(float s){this->scale = s;}
			inline int getLength(){return entry.length();}
			inline std::wstring getText(){return entry;}
			inline bool isHorizontal(){return horizontal;}
			inline void setHorizontal(bool h){this->horizontal = h;}
			inline bool isLeftToRight(){return leftToRight;}
			inline void setLeftToRight(bool ltr){this->leftToRight = ltr;}
			inline Material* getMaterial(){return material;}
			inline void setMaterial(Material *mat){this->material = mat;}
			Glyph* getGlyph(u16);
		private:
			void applyFont(std::string, u16, u16 = 256);
			void clearFont();
			void prepareGlyphs(Glyph, Vector2);
			void renderGlyphs(Glyph, float[], u32);

			Material *material = nullptr;
			std::wstring entry;
			std::vector<Glyph> characters;
			unsigned int VAO, VBO;
			float scale = 1;
			bool horizontal = true, leftToRight = true;
	};
}

#endif
