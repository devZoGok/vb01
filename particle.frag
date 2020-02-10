#version 330 core

in vec2 texCoords;

flat in int id;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 color[200];

void main(){
	float alpha=texture(tex,texCoords).r;
	vec4 c=color[id];
	//FragColor=vec4(1,0,0,1);
	FragColor=vec4(c.x,c.y,c.z,c.w*alpha);
}
