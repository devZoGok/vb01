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
			float getCharWidth(char);
			inline Node* getNode(){return node;}
			inline void setNode(Node *node){this->node=node;}
			inline void setScale(float s){this->scale=s;}
			inline void setColor(Vector4 c){this->color=c;}
			inline int getLength(){return entry.length();}
			inline std::string getText(){return entry;}
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
