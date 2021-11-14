#version 330 core
const int numLights=1;
const float PI = 3.14159;

in vec3 fragPos;
in vec3 tan;
in vec3 biTan;
in vec3 norm;
in vec2 texCoords;

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

uniform Light lights[numLights];
uniform Texture textures[1];

uniform bool albedoMapEnabled;
uniform vec4 albedoColor;
uniform float roughness;
uniform float metalness;
uniform vec3 baseReflectivity;
uniform vec3 camPos;

float trowbridgeReitz(vec3 n, vec3 h, float a){
	return a * a / (PI * pow(pow(max(dot(n, h), 0), 2) * (a * a - 1) + 1, 2));
}

float schlickGGX(vec3 vec1, vec3 vec2, float k){
	float dotProd = max(dot(vec1, vec2), 0);
	return dotProd / (dotProd * (1 - k) + k);
}

float geoSmith(vec3 n, vec3 v, vec3 l, float k){
	float ggx1 = schlickGGX(n, v, k);
	float ggx2 = schlickGGX(n, l, k);
	return ggx1 * ggx2;
}

vec3 fresnelSchlick(vec3 v, vec3 h, vec3 f0){
	return f0 + (vec3(1) - f0) * pow(1 - max(dot(v, h), 0), 5);
}

vec3 lambertDiffuse(vec4 color){
	return (color / PI).xyz;
}

void main() {
	vec4 finalColor = vec4(0, 0, 0, 1);
	vec3 normal = norm;
	vec3 viewDir = normalize(camPos - fragPos);
	vec4 albedo = (albedoMapEnabled ? texture(textures[0].pastTexture, texCoords) : albedoColor);

	for(int i = 0; i < numLights; ++i){
		vec3 lightDir;
		float dist = length(lights[i].pos - fragPos), attenuation = 1, a = lights[i].a, b = lights[i].b, c = lights[i].c;

		if(lights[i].type == 0){
			lightDir = normalize(lights[i].pos - fragPos);
			attenuation = 1 / (a * dist * dist + b * dist + c);
		}
		else if(lights[i].type == 1){
			lightDir = -normalize(lights[i].direction);
		}
		else if(lights[i].type == 2){
			lightDir = normalize(lights[i].pos - fragPos);
			attenuation = 1 / (a * dist * dist + b * dist + c);
		}

		vec3 halfVec = normalize(lightDir + normal);

		float D = trowbridgeReitz(normal, halfVec, roughness * roughness);
		float G = geoSmith(normal, halfVec, lightDir, 0.5 * roughness * roughness);
		vec3 F = fresnelSchlick(viewDir, halfVec, baseReflectivity);
		vec3 cookTor = D * G * F / max(4 * max(dot(viewDir, normal), 0) * max(dot(lightDir, normal), 0), .000001);

		vec3 fDiff = lambertDiffuse(albedo);
		vec3 kDiff = (vec3(1) - F) * (1 - metalness);

		finalColor.rgb += (kDiff * fDiff + cookTor) * lights[i].color * attenuation * max(dot(lightDir, normal), 0);
	}

	float brightness = dot(finalColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	if(brightness > 1)
		BrightColor = vec4(finalColor.rgb, 1);

	FragColor = finalColor;
}
