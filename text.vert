#version 330 core

layout (location=0) in vec4 aVert;

out vec2 texCoords;

uniform vec2 screen;
uniform vec3 pos;

void main(){
	float x,y;
	x=(aVert.x-screen.x*.5)/(screen.x*.5);
	y=(screen.y*.5-(aVert.y))/(screen.y*.5);
	gl_Position=vec4(x,y,pos.z,1);
	texCoords=aVert.zw;
}
