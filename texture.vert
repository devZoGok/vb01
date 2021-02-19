#version 330 core

const int numBones = 1;
const int numVertGroups = 1;
const int numMaxInfluences = 4;
const int numShapeKeys = 1;

mat4 transform[numBones];
mat4 poseTransform[numBones];

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in vec3 aTan;
layout (location=4) in vec3 aBiTan;
layout (location=5) in vec4 aWeight;
layout (location=6) in ivec4 aBoneIndices;
layout (location=7) in vec3[numShapeKeys] aShapeKeyOffsets;

out vec3 fragPos;
out vec3 norm;
out vec3 tan;
out vec3 biTan;
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

mat4 createTranslationMatrix(vec3 trans){
	mat4 transMat;
	transMat[0] = vec4(1, 0, 0, trans.x);
	transMat[1] = vec4(0, 1, 0, trans.y);
	transMat[2] = vec4(0, 0, 1, trans.z);
	transMat[3] = vec4(0, 0, 0, 1);
	return transpose(transMat);
}

mat4 createRotationMatrix(float angle, vec3 axis){
	mat4 rotMat;

	float s = sin(-angle);
	float c = cos(-angle);
	float oc = 1.0 - c;

	rotMat[0] = vec4(oc * axis.x * axis.x + c,oc * axis.x * axis.y + axis.z * s,oc * axis.z * axis.x - axis.y * s, 0.0);                              
	rotMat[1] = vec4(oc * axis.x * axis.y - axis.z * s,oc * axis.y * axis.y + c,oc * axis.y * axis.z + axis.x * s, 0.0);                              
	rotMat[2] = vec4(oc * axis.z * axis.x + axis.y * s,oc * axis.y * axis.z - axis.x * s,oc * axis.z * axis.z + c, 0.0);                              
	rotMat[3] = vec4(0, 0, 0, 1);

	return transpose(rotMat);
}

mat4 createScaleMatrix(vec3 scale){
	mat4 scaleMat;
	scaleMat[0] = vec4(scale.x, 0, 0, 0);
	scaleMat[1] = vec4(0, scale.y, 0, 0);
	scaleMat[2] = vec4(0, 0, scale.z, 0);
	scaleMat[3] = vec4(0, 0, 0, 1);
	return scaleMat;
}

float getWeight(Bone bone){
	float weight = 0;
	for(int i = 0; i < numMaxInfluences; i++){
		if(bone.vertGroup == aBoneIndices[i])
			weight = aWeight[i];
	}
	return weight;
}

void main(){
	vec4 vertPos = vec4(aPos, 1);
	bool anyWeights = false;
	for(int i = 0; i < numMaxInfluences; i++)
		if(aWeight[i] > 0)
			anyWeights = true;

	if(animated && anyWeights){
		for(int i = 0; i < numBones; i++){
			transform[i] = createTranslationMatrix(bones[i].pos);
			vec3 trans = bones[i].trans;
			float angle = bones[i].angle;
			vec3 axis = bones[i].rotAxis;
			vec3 scale = bones[i].scale;
			mat4 animTransform = createTranslationMatrix(trans) * createRotationMatrix(angle, axis) * createScaleMatrix(scale);
			poseTransform[i] = transform[i] * animTransform;
		}

		for(int i = 1; i < numBones; i++){
			mat4 localTransform = inverse(transform[bones[i].parent]) * poseTransform[i];
			poseTransform[i] = poseTransform[bones[i].parent] * localTransform;
		}

		vec3 v0 = vec3(0);
		for(int i = 0; i < numBones; i++){
			float weight = getWeight(bones[i]);
			vec4 boneLocalVert = inverse(transform[i]) * vertPos;
			v0 += (weight * poseTransform[i] * boneLocalVert).xyz; 

		}
		vertPos.xyz = v0;
	}

	for(int i = 0; i < numShapeKeys; i++){
		vertPos.xyz += aShapeKeyOffsets[i] * shapeKeyFactors[i];
	}

	gl_Position = proj * view * model * vertPos;
	fragPos = vec3(model * vertPos);
	norm = mat3(transpose(inverse(model))) * aNorm;
	tan = mat3(transpose(inverse(model))) * aTan;
	biTan = mat3(transpose(inverse(model))) * aBiTan;
	texCoords = aTexCoords;
}
