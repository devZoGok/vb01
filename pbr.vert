#version 330 core

const int numBones = 1;
const int numVertGroups = 1;
const int numMaxInfluences = 4;
const int numShapeKeys = 1;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTan;
layout (location = 4) in vec3 aBiTan;

out vec3 fragPos, norm, tan, biTan;
out vec2 texCoords;

struct Bone{
	float angle;
	vec3 pos, trans, rotAxis, scale;
	int vertGroup, parent;
};

uniform bool animated;
uniform Bone bones[numBones];
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float[numShapeKeys] shapeKeyFactors; 

void main(){
	mat3 normalMat = mat3(transpose(inverse(model)));
	norm = (normalMat * aNorm);
	tan = (normalMat * aTan);
	biTan = (normalMat * aBiTan);
	fragPos = vec3(model * vec4(aPos, 1));
	texCoords = aTexCoords;

	gl_Position = proj * view * model * vec4(aPos, 1);
}
