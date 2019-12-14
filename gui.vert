#version 330 core

layout (location=0) in vec3 aVert;
layout (location=2) in vec2 aTexCoords;

out vec2 texCoords;

uniform bool spatialToScreen;
uniform vec2 screen;
uniform vec3 pos;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main(){
	if(!spatialToScreen){
		float x,y;
		x=(aVert.x+pos.x-screen.x*.5)/(screen.x*.5);
		y=(screen.y*.5-(aVert.y+pos.y))/(screen.y*.5);
		gl_Position=vec4(x,y,pos.z+aVert.z,1);
	}
	else{
		vec3 pos=vec3(proj*view*model*vec4(aVert,1));
		gl_Position=vec4(pos.xy,aVert.z,1);
	}
	texCoords=aTexCoords;
}
