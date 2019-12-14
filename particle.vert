#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoords;

out vec2 texCoords;

flat out int id;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model[100];
uniform vec2 size[100];

void main(){
	vec3 camLeft=-vec3(view[0][0],view[1][0],view[2][0]);
	vec3 camUp=vec3(view[1][0],view[2][0],view[3][0]);
	id=gl_InstanceID;
	vec2 s=size[gl_InstanceID];
	gl_Position=proj*view*model[gl_InstanceID]*vec4(camLeft*s.x*aPos.x+camUp*s.y*aPos.y+camDir*aPos.z,1);	
	texCoords=aTexCoords;
}
