#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projView;

void main(){
	texCoords = aPos;
 	vec4 pos = projView * vec4(aPos, 1);
	gl_Position = pos;
}
