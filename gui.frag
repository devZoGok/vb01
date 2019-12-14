#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform bool texturingEnabled;
uniform bool diffuseColorEnabled;
uniform vec4 diffuseColor;
uniform sampler2D tex;

void main(){
	vec4 c=texturingEnabled?texture(tex,texCoords):diffuseColor;
	if(diffuseColorEnabled){
		float alpha=texture(tex,texCoords).w;
		c=vec4(diffuseColor.xyz,alpha);
	}
	FragColor=c;
}
