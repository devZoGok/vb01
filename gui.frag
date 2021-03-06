#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform bool texturingEnabled;
uniform bool diffuseColorEnabled;
uniform bool guiPlane;
uniform vec4 diffuseColor;
uniform sampler2D tex;

void main(){
	vec4 c = texturingEnabled?texture(tex, texCoords) : diffuseColor;
	if(diffuseColorEnabled){
		float alpha = texture(tex, texCoords).w;
		c = vec4(diffuseColor.xyz, alpha);
	}
	if(guiPlane){
		float gamma = 1, exposure = 1;

		//Reinhardt
		//c.rgb = c.rgb / (c.rgb + vec3(1.));

		//Exp
		c.rgb = vec3(1.) - exp(-c.rgb * exposure);

		c.rgb = pow(c.rgb, vec3(1 / gamma));
	}
	FragColor = c;
}
