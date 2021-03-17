#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in mat4 aInstancedModel;

out vec2 texCoords;

flat out int id;

const int numParticles = 1;

uniform mat4 proj, view;
uniform float lifePercentage[numParticles];
uniform vec3 trans[numParticles];
uniform vec2 startSize, endSize;

void main(){
	vec3 camLeft = vec3(view[0][0], view[1][0], view[2][0]);	
	vec3 camUp = vec3(view[0][1], view[1][1], view[2][1]);	

	id = gl_InstanceID;
	vec2 s = mix(startSize, endSize, lifePercentage[id]);
	vec3 v = camLeft * s.x * aPos.x + camUp * s.y * aPos.y;
	mat4 model = aInstancedModel;
	model[3][0] = trans[id].x;
	model[3][1] = trans[id].y;
	model[3][2] = trans[id].z;

	gl_Position = proj * view * model * vec4(v, 1);	
	texCoords = aTexCoords;
}
