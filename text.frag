#version 330 core

in vec2 texCoords;

out vec4 FragColor;

struct Texture{
	float mixRatio;
	sampler2D pastTexture, nextTexture;
	bool animated;
};

uniform Texture textures[2];
uniform bool texturingEnabled;
uniform vec4 diffuseColor;

void main(){
	vec4 diffCol;
	if(texturingEnabled){
		diffCol = texture(textures[0].pastTexture, texCoords);
		if(textures[0].animated)
			diffCol = mix(diffCol, texture(textures[0].nextTexture, texCoords), textures[0].mixRatio);
	}
	else
		diffCol = diffuseColor;

	float alpha = texture(textures[1].pastTexture, texCoords).r;
	FragColor = vec4(diffCol.xyz, diffCol.w * alpha);
}
