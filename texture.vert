#version 330 core

const int numBones=1;
const int numVertGroups=1;
const int numMaxInfluences = 4;

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
	int vertGroup,parent;
};

uniform bool animated;
uniform Bone bones[numBones];
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
	vec4 vertPos=vec4(aPos,1);
	if(animated){
		for(int i = 0; i < numBones; i++){
			Bone bone = bones[i];
			float weight = 0;

			for(int j = 0; j < numMaxInfluences; j++){
				if(bone.vertGroup == aBoneIndices[j])
					weight = aWeight[j];
			}	

			while(bone.parent!=-1){
				float angle = weight * bone.angle;
				vec3 trans = bone.trans;
				vec3 axis = bone.rotAxis;
				vec3 scale = bone.scale;
				vec4 bonePos = vec4(bone.pos, 1);
			
				float s = sin(angle);
				float c = cos(angle);
				float oc = 1.0 - c;
			
				mat4 rotMat;
				rotMat[0] = vec4(oc * axis.x * axis.x + c,oc * axis.x * axis.y + axis.z * s,oc * axis.z * axis.x - axis.y * s, 0.0);                              
				rotMat[1] = vec4(oc * axis.x * axis.y - axis.z * s,oc * axis.y * axis.y + c,oc * axis.y * axis.z + axis.x * s, 0.0);                              
				rotMat[2] = vec4(oc * axis.z * axis.x + axis.y * s,oc * axis.y * axis.z - axis.x * s,oc * axis.z * axis.z + c, 0.0);                              
				rotMat[3] = vec4(0, 0, 0, 1);
				
				vertPos += -(vertPos - bonePos) + rotMat * (vertPos - bonePos);
				vec4 vertToBone = vec4(vertPos.xyz - bonePos.xyz, 1);
				vertPos.xyz += -weight * vertToBone.xyz + weight * vec3(scale.x * vertToBone.x, scale.y * vertToBone.y, scale.z * vertToBone.z);
				vertPos.xyz += weight * trans;

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
