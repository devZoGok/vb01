#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform uint numSamples;

const float PI = 3.14159265359;

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float radicalInverseVdC(uint bits) {
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint normal) {
	return vec2(float(i) / float(normal), radicalInverseVdC(i));
}

vec3 importanceSampleGGX(vec2 Xi, vec3 normal, float a) {
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
	vec3 halfVec = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
	
	// from tangent-space H vector to world-space sample vector
	vec3 up = (abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0));
	vec3 tan = normalize(cross(up, normal));
	vec3 biTan = cross(normal, tan);
	
	vec3 sampleVec = tan * halfVec.x + biTan * halfVec.y + normal * halfVec.z;
	return normalize(sampleVec);
}

float schlickGGX(float nDotV, float k) {
    return nDotV / (nDotV * (1.0 - k) + k);
}

float geoSmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
    float nDotV = max(dot(normal, viewDir), 0.0);
    float nDotL = max(dot(normal, lightDir), 0.0);
		float k = pow(roughness, 4) / 2.0; 
    float ggx2 = schlickGGX(nDotV, k);
    float ggx1 = schlickGGX(nDotL, k);

    return ggx1 * ggx2;
}

vec2 integrateBRDF(float nDotV, float roughness) {
    vec3 viewDir = vec3(sqrt(1.0 - nDotV * nDotV), 0.0, nDotV);

    float a = 0.0;
    float b = 0.0; 

    vec3 normal = vec3(0.0, 0.0, 1.0);

    for(uint i = 0u; i < numSamples; ++i) {
        // generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
        vec2 Xi = hammersley(i, numSamples);
        vec3 halfVec = importanceSampleGGX(Xi, normal, roughness);
        vec3 lightDir = normalize(2.0 * dot(viewDir, halfVec) * halfVec - viewDir);

        float nDotL = max(lightDir.z, 0.0);
        float nDotH = max(halfVec.z, 0.0);
        float vDotH = max(dot(viewDir, halfVec), 0.0);

        if(nDotL > 0.0) {
            float G = geoSmith(normal, viewDir, lightDir, roughness);
            float gVis = (G * vDotH) / (nDotH * nDotV);
            float fc = pow(1.0 - vDotH, 5.0);

            a += (1.0 - fc) * gVis;
            b += fc * gVis;
        }
    }

    a /= float(numSamples);
    b /= float(numSamples);
    return vec2(a, b);
}

void main() {
    vec2 integratedBRDF = integrateBRDF(texCoords.x, texCoords.y);
    FragColor = vec4(integratedBRDF, 0, 1);
}
