#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoords;

out vec2 texCoords;

flat out int id;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model[100];
uniform vec2 size[100];
uniform vec3 camDir;
uniform vec3 camLeft;
uniform vec3 camUp;

void main(){
	id=gl_InstanceID;
	vec2 s=size[gl_InstanceID];
	gl_Position=proj*view*model[gl_InstanceID]*vec4(camLeft*s.x*aPos.x+camUp*s.y*aPos.y+camDir*aPos.z,1);	
	texCoords=aTexCoords;
}
