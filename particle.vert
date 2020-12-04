#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in mat4 aInstancedModel;

out vec2 texCoords;

flat out int id;

uniform mat4 proj;
uniform mat4 view;
uniform float lifePercentage[500];
uniform vec3 trans[500];
uniform vec2 startSize;
uniform vec2 endSize;

void main(){
	vec3 camLeft = vec3(view[0][0], view[1][0], view[2][0]);	
	vec3 camUp = vec3(view[0][1], view[1][1], view[2][1]);	

	id = gl_InstanceID;
	vec2 s = startSize + (endSize - startSize) * lifePercentage[id];
	vec3 v = camLeft * s.x * aPos.x+camUp * s.y * aPos.y;
	mat4 model = aInstancedModel;
	model[3][0] = trans[gl_InstanceID].x;
	model[3][1] = trans[gl_InstanceID].y;
	model[3][2] = trans[gl_InstanceID].z;

	gl_Position = proj * view * model * vec4(v, 1);	
	texCoords = aTexCoords;
}
