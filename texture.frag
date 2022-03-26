#version 330 core
const int numLights=1;
const bool checkLights=false;

in vec3 fragPos;
in vec3 tan;
in vec3 biTan;
in vec3 norm;
in vec2 texCoords;
in vec4 lightSpaceFragPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

//0-POINT,1-DIRECTIONAL,2-SPOT

struct Light{
	int type;
	vec3 pos, color, direction;
	float innerAngle, outerAngle;
	float a, b, c, near, far;
	sampler2D depthMap;
	samplerCube depthMapCube;
	mat4 lightMat;
};

struct Texture{
	float mixRatio;
	sampler2D pastTexture, nextTexture;
	bool animated;
};

uniform Texture textures[4];
uniform samplerCube environmentMap;
uniform Light lights[numLights];
uniform bool lightingEnabled, texturingEnabled, normalMapEnabled, specularMapEnabled, castShadow, environmentMapEnabled;
uniform vec4 diffuseColor, specularColor;
uniform float shinyness, specularStrength;
uniform vec3 camPos;

float linDepth(float depth, int i){
	float z = depth * 2 - 1, near = lights[i].near, far = lights[i].far;
	return (2 * near * far) / (far + near - z * (far - near));
}

float getShadow(int id){
	int type = lights[id].type;

	float shadow = 0, closestDepth = 0, currentDepth = 0, bias = .005, val = .7;

	if(type == 0){
		vec3 projDir = fragPos - lights[id].pos;
		closestDepth = texture(lights[id].depthMapCube, projDir).r * lights[id].far;
		currentDepth = length(projDir);
		shadow = (currentDepth - bias > closestDepth) ? val : 0;
	}
	else{
		vec4 lightSpaceFragPos = lights[id].lightMat * vec4(fragPos, 1);
		vec3 projCoords = (lightSpaceFragPos.xyz / lightSpaceFragPos.w) * .5 + .5;
		closestDepth = texture(lights[id].depthMap, projCoords.xy).r;
		currentDepth = projCoords.z;

		if(type == 2)
			closestDepth = linDepth(closestDepth, id); currentDepth = linDepth(currentDepth, id);

		shadow = (currentDepth - bias > closestDepth) ? val : 0;

		if(projCoords.z > 1)
			shadow = 0;
	}

	return shadow;
}

void main(){
	vec4 finalColor = diffuseColor;

	if(texturingEnabled){
		vec4 textureColor = texture(textures[0].pastTexture, texCoords); 
		vec4 mixedTexColor = mix(textureColor, texture(textures[0].nextTexture, texCoords), textures[0].mixRatio);
		finalColor = (textures[0].animated ? mix(textureColor, texture(textures[0].nextTexture, texCoords), textures[0].mixRatio) : textureColor);
	}

	vec3 normal = norm;

	if(lightingEnabled){
		if(normalMapEnabled){
			vec3 n = normalize(vec3(texture(textures[1].pastTexture, texCoords)) * 2 - 1);

			if(textures[1].animated)
				n = mix(n, normalize(texture(textures[1].nextTexture, texCoords).rgb * 2 - 1), textures[1].mixRatio);

			normal = mat3(tan, biTan, norm) * n;
		}

		vec3 diffuseCol = vec3(0), specularCol = vec3(0);

		if(checkLights){
			for(int i = 0; i < numLights; i++){
				vec3 lightDir = vec3(0), viewDir = normalize(camPos - fragPos);
				float attenuation = 1.0;
				float a = lights[i].a, b = lights[i].b, c = lights[i].c, dist = length(fragPos - lights[i].pos), factor, coef = 1;

				if(lights[i].type == 0){
					lightDir = normalize(lights[i].pos - fragPos);
					attenuation = 1.0 / (a * dist * dist + b * dist + c);
				}
				else if(lights[i].type == 1)
					lightDir = -normalize(lights[i].direction);
				else if(lights[i].type == 2){
					lightDir = normalize(lights[i].pos - fragPos);
					float innerAngle = lights[i].innerAngle;
					float angle = acos(dot(-lightDir, lights[i].direction));
					float outerAngle = lights[i].outerAngle;

					if(lights[i].innerAngle <= angle && angle <= lights[i].outerAngle)
						coef = (angle - outerAngle) / (innerAngle - outerAngle);

					if(angle > lights[i].outerAngle)
						continue;
				}

				factor = max(dot(lightDir, normal), 0.);
				diffuseCol += lights[i].color * (factor * attenuation * coef);

				if(specularMapEnabled){
					float specularSample = texture(textures[2].pastTexture, texCoords).r;
					vec3 halfwayVec = normalize(lightDir + viewDir);
					float spec = pow(max(dot(normal, halfwayVec), 0), shinyness);	
					specularCol += specularStrength * spec * specularSample * lights[i].color;
				}

				float shadow = getShadow(i);
				diffuseCol *= (1.0 - shadow);
			}

			finalColor *= vec4(diffuseCol + specularCol, 1);
		}
	}

	float brightness = dot(finalColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	if(brightness > 1)
		BrightColor = vec4(finalColor.rgb, 1);

	FragColor = finalColor;
}
