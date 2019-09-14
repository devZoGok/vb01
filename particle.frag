#version 330 core

in vec2 texCoords;

flat in int id;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 color[100];

void main(){
	float alpha=texture(tex,texCoords).x;
	vec4 c=color[id];
	FragColor=vec4(c.x,c.y,c.z,c.w*alpha);
}
