#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 proj, view;

void main(){
	texCoords = aPos;
 	vec4 pos = proj * view * vec4(aPos, 1);
	gl_Position = pos;
}
