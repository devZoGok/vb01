#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform bool texturingEnabled;
uniform vec4 diffuseColor;
uniform sampler2D tex;

void main(){
	FragColor=texturingEnabled?texture(tex,texCoords):diffuseColor;
}
