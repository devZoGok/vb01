#version 330 core

in vec2 texCoords;

flat in int id;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D tex;
uniform float lifePercentage[500];
uniform vec4 startColor, endColor;

void main(){
	float alpha = texture(tex, texCoords).r;
	vec4 c = startColor + (endColor - startColor) * lifePercentage[id];
	c = vec4(c.x, c.y, c.z, alpha);
	float brightness = dot(c.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1)
		BrightColor = vec4(c.rgb, 1);
	FragColor = c;
}
