#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D textTex, diffuseMap;
uniform vec4 diffuseColor;

void main(){
	float alpha = texture(textTex, texCoords).r;
	FragColor = vec4(diffuseColor.xyz, diffuseColor.w * alpha);
}
