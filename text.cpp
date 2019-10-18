#include"root.h"
#include"text.h"
#include"shader.h"
#include"texture.h"
#include"node.h"
#include<glad.h>
#include<glfw3.h>
#include<iostream>
#include<ft2build.h>
#include FT_FREETYPE_H

using namespace std;

namespace vb01{
	Text::Text(string fontPath,string entry){
		string basePath="/home/dominykas/c++/vb01/text.";
		shader=new Shader(basePath+"vert",basePath+"frag");

		FT_Library ft;
		FT_Face face;
		if(FT_Init_FreeType(&ft))
			cout<<"Couldn't init Freetype\n";	
		if(FT_New_Face(ft,fontPath.c_str(),0,&face))
			cout<<"Could not load font\n";

		FT_Set_Pixel_Sizes(face,0,100);

		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

		for(u32 i=0;i<256;i++){
			if(FT_Load_Char(face,i,FT_LOAD_RENDER)){
				cout<<"Could not load glyph\n";
				continue;
			}
			Glyph c;
			c.ch=i;
			c.texture=new Texture(face,i);		
			c.size=Vector2(face->glyph->bitmap.width,face->glyph->bitmap.rows);
			c.bearing=Vector2(face->glyph->bitmap_left,face->glyph->bitmap_top);
			c.advance=face->glyph->advance.x;
			characters.push_back(c);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		setText(entry);
	}

	Text::~Text(){}

	void Text::update(){
		Root *root=Root::getSingleton();
		int width=root->getWidth(),height=root->getHeight();

		shader->use();
		shader->setVec4(color,"color");
		shader->setVec2(Vector2(width,height),"screen");
		shader->setVec3(node->getPosition(),"pos");

		Vector3 origin=node->getPosition();
		for(int i=0;i<entry.length();i++){
			Glyph ch;
			bool foundChar=false;
			for(Glyph c : characters)
				if(entry.c_str()[i]==c.ch){
					ch=c;
					foundChar=true;
				}
			if(!foundChar)continue;
			Vector2 size=ch.size*scale,bearing=ch.bearing*scale;
			float data[]={
				origin.x+bearing.x,origin.y-bearing.y,0,0,
				origin.x+bearing.x+size.x,origin.y-bearing.y,1,0,
				origin.x+bearing.x+size.x,origin.y-bearing.y+size.y,1,1,

				origin.x+bearing.x+size.x,origin.y-bearing.y+size.y,1,1,
				origin.x+bearing.x,origin.y-bearing.y+size.y,0,1,
				origin.x+bearing.x,origin.y-bearing.y,0,0
			};
			origin.x+=(ch.advance>>6)*scale;
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(data),data);
			ch.texture->select();
			glDrawArrays(GL_TRIANGLES,0,6);	
		}
	}

	void Text::setText(string text){
		this->entry=text;

		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*6*4,NULL,GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);
	}

	float Text::getCharWidth(char c){
		for(Glyph g : characters)
			if(g.ch==c)
				return g.size.x*scale;
	}
}
