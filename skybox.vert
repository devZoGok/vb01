#version 330 core

layout (location=0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 proj;
uniform mat4 view;

void main(){
	texCoords=aPos;
 	gl_Position=proj*view*vec4(aPos,1);
}
