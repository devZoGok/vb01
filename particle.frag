#version 330 core

in vec2 texCoords;

flat in int id;

const int numParticles = 1;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Texture{
	float mixRatio;
	sampler2D pastTexture, nextTexture;
	bool animated;
};

uniform Texture tex;
uniform float lifePercentage[numParticles];
uniform vec4 startColor, endColor;

void main(){
	float alpha = texture(tex.pastTexture, texCoords).r;
	if(tex.animated)
		alpha = mix(alpha, texture(tex.nextTexture, texCoords).r, tex.mixRatio);

	vec4 c = (alpha > 0 ? mix(startColor, endColor, alpha) : vec4(0));
	FragColor = c;

	float brightness = dot(c.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1)
		BrightColor = vec4(c.rgb, 1);
}
