#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 proj, view;

void main(){
	gl_Position = proj * view * vec4(aPos, 1);
}
