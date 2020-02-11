#version 330 core

const int numLights=1;
const int numShadows=1;

in vec3 fragPos;
in vec3 norm;
in vec2 texCoords;
in vec4 lightSpaceFragPos;

out vec4 FragColor;

//0-POINT,1-DIRECTIONAL,2-SPOT

struct Light{
	int type;
	vec3 pos,color,direction;
	float innerAngle,outerAngle;
	float a,b,c;
	sampler2D depthMap[numShadows];
	mat4 lightMat;
};

uniform sampler2D tex;
uniform Light light[numLights];
uniform bool lightingEnabled;
uniform bool texturingEnabled;
uniform bool castShadow;
uniform vec4 diffuseColor;
uniform vec3 camPos;

float getShadow(int id,int shadowId){
	vec3 projCoords=(light[id].lightMat*vec4(fragPos,1)).xyz/(light[id].lightMat*vec4(fragPos,1)).w;
	projCoords=projCoords*.5+.5;
	float closestDepth=texture(light[id].depthMap[shadowId],projCoords.xy).r;
	float currentDepth=projCoords.z;
	float bias=.005;
	return (currentDepth-bias>closestDepth)&&projCoords.z<=1.?.7:0;
}

void main(){
	vec4 finalColor=texturingEnabled?texture(tex,texCoords):diffuseColor;
	if(lightingEnabled){
		vec3 diffuseColor=vec3(0),specularColor;
		float coef=1;
		for(int i=0;i<numLights;i++){
			vec3 specularColor,lightDir,reflectVec,viewDir=normalize(camPos-fragPos);
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
				float angle=acos(dot(lightDir,norm));
				float outerAngle=light[i].outerAngle;
				if(light[i].innerAngle<=angle&&angle<=light[i].outerAngle){
					coef=(angle-outerAngle)/(innerAngle-outerAngle);
				}
				if(angle>light[i].outerAngle){
					coef=0;	
				}
			}
			float specularStrength=.5;
			reflectVec=reflect(lightDir,norm);
			float spec=pow(max(dot(viewDir,reflectVec),0),32);	
			specularColor+=vec3(1)*spec*specularStrength;

			factor=max(dot(lightDir,norm),0.);
			diffuseColor+=light[i].color*(factor*attenuation*coef);
		}	

		for(int i=0;i<numLights;i++)
			for(int j=0;j<numShadows;j++){
				float shadow=coef*getShadow(i,j);
				diffuseColor*=(1-shadow);
			}

		finalColor*=vec4(diffuseColor+specularColor,1);
	}
	FragColor=finalColor;
}
