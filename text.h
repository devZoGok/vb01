#ifndef TEXT_H
#define TEXT_H

#include<string>
#include<vector>
#include"vector.h"
#include"util.h"

namespace vb01{
	class Node;
	class Texture;
	class Shader;

	class Text{
		public:
			struct Glyph{
				u32 ch;
				Texture *texture=nullptr;
				unsigned int advance;
				Vector2 size,bearing;
			};

			Text(std::string,std::string);
			~Text();
			void update();
			void setText(std::string);
			inline void setNode(Node *node){this->node=node;}
		private:
			Node *node=nullptr;
			std::string entry;
			std::vector<Glyph> characters;
			Shader *shader=nullptr;
			unsigned int VAO, VBO;
			float scale=1;
			Vector4 color=Vector4::VEC_IJKL;
	};
}

#endif
