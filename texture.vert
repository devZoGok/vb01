#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in vec3 aTan;
layout (location=4) in vec3 aBiTan;

out vec3 fragPos;
out vec3 norm;
out vec3 tan;
out vec3 biTan;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
	gl_Position=proj*view*model*vec4(aPos,1);
	fragPos=vec3(model*vec4(aPos,1));
	norm=mat3(transpose(inverse(model)))*aNorm;
	tan=mat3(transpose(inverse(model)))*aTan;
	biTan=mat3(transpose(inverse(model)))*aBiTan;
	texCoords=aTexCoords;
}
