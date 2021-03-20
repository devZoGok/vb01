#version 330 core

in vec3 texCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform samplerCube skybox;

void main(){
	BrightColor = vec4(0);
	FragColor = texture(skybox, texCoords);	
}
