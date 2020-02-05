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
	vec3 camLeft=vec3(view[0][0],view[1][0],view[2][0]);	
	vec3 camUp=vec3(view[0][1],view[1][1],view[2][1]);	

	id=gl_InstanceID;
	vec2 s=size[gl_InstanceID];
	vec3 v=camLeft*s.x*aPos.x+camUp*s.y*aPos.y;
	gl_Position=proj*view*model[gl_InstanceID]*vec4(v,1);	
	//gl_Position=proj*view*model[gl_InstanceID]*vec4(s.x*aPos.x,s.y*aPos.y,aPos.z,1);	
	texCoords=aTexCoords;
}
