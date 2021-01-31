#version 330 core

const int numBones=1;
const int numVertGroups=1;
const int numMaxInfluences = 4;

mat4 transform[numBones];

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;
layout (location=2) in vec2 aTexCoords;
layout (location=3) in vec3 aTan;
layout (location=4) in vec3 aBiTan;
layout (location=5) in vec4 aWeight;
layout (location=6) in ivec4 aBoneIndices;

out vec3 fragPos;
out vec3 norm;
out vec3 tan;
out vec3 biTan;
out vec2 texCoords;

struct Bone{
	float angle;
	vec3 pos,trans,rotAxis,scale;
	int vertGroup,parent,id;
};

uniform bool animated;
uniform Bone bones[numBones];
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

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
	if(animated){
		for(int i = 0; i < numBones; i++){
			transform[i] = createTranslationMatrix(bones[i].pos);
		}

		for(int i = 0; i < numBones; i++){
			Bone bone = bones[i];
			float weight = getWeight(bone);
			while(bone.parent != -1){
				vec3 trans = bone.trans;
				float angle = bone.angle;
				vec3 axis = bone.rotAxis;
				vec3 scale = bone.scale;
				
				mat4 animTransform = createTranslationMatrix(weight * trans) * createRotationMatrix(weight * angle, axis);

				vec4 boneLocalVert = inverse(transform[bone.id]) * vertPos;
				vertPos = (transform[bone.id] * animTransform * boneLocalVert); 
				
				bone = bones[bone.parent];
			}
		}
	}

	gl_Position = proj * view * model * vertPos;
	fragPos = vec3(model * vec4(aPos, 1));
	norm = mat3(transpose(inverse(model))) * aNorm;
	tan = mat3(transpose(inverse(model))) * aTan;
	biTan = mat3(transpose(inverse(model))) * aBiTan;
	texCoords = aTexCoords;
}
