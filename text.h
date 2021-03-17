#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>
#include "vector.h"
#include "util.h"

namespace vb01{
	class Node;
	class Texture;
	class Shader;

	class Text{
		public:
			struct Glyph{
				u16 ch;
				Texture *texture = nullptr;
				u32 advance;
				Vector2 size, bearing;
			};

			Text(std::string, std::wstring);
			~Text();
			void update();
			void setText(std::wstring);
			inline Node* getNode(){return node;}
			inline void setNode(Node *node){this->node = node;}
			inline void setScale(float s){this->scale = s;}
			inline void setColor(Vector4 c){this->color = c;}
			inline int getLength(){return entry.length();}
			inline std::wstring getText(){return entry;}
			inline bool isHorizontal(){return horizontal;}
			inline void setHorizontal(bool h){this->horizontal = h;}
		private:
			void initFont(std::string);
			void prepareGlyphs(Glyph, Vector2);
			void renderGlyphs(Glyph, float[], u32);
			Glyph* getGlyph(u16);

			Node *node = nullptr;
			std::wstring entry;
			std::vector<Glyph> characters;
			Shader *shader = nullptr;
			unsigned int VAO, VBO;
			float scale = 1;
			bool horizontal = true;
			Vector4 color = Vector4::VEC_IJKL;
	};
}

#endif
