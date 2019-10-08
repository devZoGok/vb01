#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 color;

void main(){
	//FragColor=vec4(1,0,1,1);
	FragColor=vec4(color.xyz,texture(tex,texCoords).r);
}
