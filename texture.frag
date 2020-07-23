#version 330 core
const int numLights=1;

in vec3 fragPos;
in vec3 tan;
in vec3 biTan;
in vec3 norm;
in vec2 texCoords;
in vec4 lightSpaceFragPos;

layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 BrightColor;

//0-POINT,1-DIRECTIONAL,2-SPOT

struct Light{
	int type;
	vec3 pos,color,direction;
	float innerAngle,outerAngle;
	float a,b,c,near,far;
	sampler2D depthMap;
	samplerCube depthMapCube;
	mat4 lightMat;
};

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D parallaxMap;
uniform samplerCube environmentMap;
uniform Light light[numLights];
uniform bool lightingEnabled;
uniform bool texturingEnabled;
uniform bool normalMapEnabled;
uniform bool castShadow;
uniform bool environmentMapEnabled;
uniform vec4 diffuseColor;
uniform float specularStrength;
uniform vec3 camPos;

float linDepth(float depth,int i){
	float z=depth*2-1,near=light[i].near,far=light[i].far;
	return (2*near*far)/(far+near-z*(far-near));
}

float getShadow(int id){
	int type=light[id].type;

	float shadow=0,closestDepth=0,currentDepth=0,bias=.005,val=.7;
	if(type==0){
		vec3 projDir=fragPos-light[id].pos;
		closestDepth=texture(light[id].depthMapCube,projDir).r*light[id].far;
		currentDepth=length(projDir);
		shadow=(currentDepth-bias>closestDepth)?val:0;
	}
/*
*/
	else{
		vec3 projCoords=(light[id].lightMat*vec4(fragPos,1)).xyz/(light[id].lightMat*vec4(fragPos,1)).w;
		projCoords=projCoords*.5+.5;
		closestDepth=texture(light[id].depthMap,projCoords.xy).r;
		currentDepth=projCoords.z;
		if(type==2){
			closestDepth=linDepth(closestDepth,id);
			currentDepth=linDepth(currentDepth,id);
		}
		shadow=(currentDepth-bias>closestDepth)?val:0;
		if(projCoords.z>1)
			shadow=0;
	}
	return shadow;
}

void main(){
	vec4 finalColor=texturingEnabled?texture(diffuseMap,texCoords):diffuseColor;
	vec3 normal=norm;
	if(lightingEnabled){
		if(normalMapEnabled){
			vec3 n=vec3(texture(normalMap,texCoords));
			normal=mat3(tan,biTan,norm)*n;
		}

		vec3 diffuseColor=vec3(0),specularColor=vec3(0);
		float coef=1;


		for(int i=0;i<numLights;i++){
			vec3 lightDir=vec3(0),reflectVec,viewDir=normalize(camPos-fragPos);
			float attenuation=1.0;
			float factor;
			float a=light[i].a,b=light[i].b,c=light[i].c,dist=length(fragPos-light[i].pos);
			coef=1;
			if(light[i].type==0){
				lightDir=normalize(light[i].pos-fragPos);
				attenuation=1.0/(a*dist*dist+b*dist+c);
			}
			else if(light[i].type==1){
				lightDir=-normalize(light[i].direction);
			}
			else if(light[i].type==2){
				lightDir=normalize(light[i].pos-fragPos);
				float innerAngle=light[i].innerAngle;
				float angle=acos(dot(-lightDir,light[i].direction));
				float outerAngle=light[i].outerAngle;
				if(light[i].innerAngle<=angle&&angle<=light[i].outerAngle){
					coef=(angle-outerAngle)/(innerAngle-outerAngle);
				}
				if(angle>light[i].outerAngle){
					coef=0;	
				}
			}
			float specularSample=texture(specularMap,texCoords).r;
			reflectVec=reflect(lightDir,normal);
			float spec=pow(max(dot(viewDir,reflectVec),0),32);	
			specularColor+=vec3(1)*spec*specularSample;

			factor=max(dot(lightDir,normal),0.);
			diffuseColor+=light[i].color*(factor*attenuation*coef);
		}	

/*
		for(int i=0;i<numLights;i++){
			float shadow=getShadow(i);
			diffuseColor*=(1-shadow);
		}
*/
		if(environmentMapEnabled){
			vec3 projDir=normalize(fragPos-camPos);
			projDir=reflect(projDir,norm);
			vec3 sample=texture(environmentMap,projDir).rgb;
			diffuseColor.rgb+=sample;
		}
		finalColor*=vec4(diffuseColor+specularColor,1);
	}

	float brightness = dot(finalColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness>1)
		BrightColor=vec4(finalColor.rgb,1);
	FragColor=finalColor;
}
