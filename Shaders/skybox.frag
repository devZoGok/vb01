#version 330 core

in vec3 texCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Texture{
	float mixRatio;
	samplerCube pastTexture, nextTexture;
	bool animated;
};

uniform Texture tex;

void main(){
	BrightColor = vec4(0);
	//vec4 texColor = (tex.animated ? mix(tex.pastTexture, tex.nextTexture, tex.mixRatio) : tex.nextTexture);
	FragColor = texture(tex.pastTexture, texCoords);	
}
