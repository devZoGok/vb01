#version 330 core

in vec2 texCoords;

flat in int id;

const int numParticles = 500;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D tex;
uniform float lifePercentage[numParticles];
uniform vec4 startColor, endColor;

void main(){
	float alpha = texture(tex, texCoords).r;
	vec4 c = mix(startColor, endColor, lifePercentage[id]);
	c = vec4(c.xyz, alpha);
	FragColor = c;

	float brightness = dot(c.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1)
		BrightColor = vec4(c.rgb, 1);
}
