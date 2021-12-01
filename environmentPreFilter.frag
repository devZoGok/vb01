#version 330 core
#
out vec4 FragColor;
in vec3 fragPos;

uniform samplerCube environmentMap;
uniform float roughness;
uniform float resolution;
uniform uint numSamples;

const float PI = 3.14159265359;

float trowbridgeReitz(vec3 normal, vec3 halfVec, float alpha) {
    float nDotH = max(dot(normal, halfVec), 0.0);
    return (alpha * alpha) / (PI * pow(nDotH * nDotH * (alpha * alpha - 1) + 1, 2));
}

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
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
	vec3 halfVec = vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
	
	// from tangent-space H vector to world-space sample vector
	vec3 up = (abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0));
	vec3 tan = normalize(cross(up, normal));
	vec3 bitan = cross(normal, tan);
	
	vec3 sampleVec = tan * halfVec.x + bitan * halfVec.y + normal * halfVec.z;
	return normalize(sampleVec);
}

void main() {		
    vec3 normal = normalize(fragPos);
    
    // make the simplyfying assumption that V equals R equals the normal 
    vec3 reflVec = normal;
    vec3 viewVec = reflVec;

    vec3 prefilteredColor = vec3(0.0);
    float totalWeight = 0.0;
    
    for(uint i = 0u; i < numSamples; ++i) {
        // generates a sample vector that's biased towards the preferred alignment direction (importance sampling).
        vec2 Xi = hammersley(i, numSamples);
        vec3 halfVec = importanceSampleGGX(Xi, normal, roughness * roughness);
        vec3 lightVec = normalize(2.0 * dot(viewVec, halfVec) * halfVec - viewVec);
        float nDotL = max(dot(normal, lightVec), 0.0);

        if(nDotL > 0.0) {
            // sample from the environment's mip level based on roughness/pdf
            float D = trowbridgeReitz(normal, halfVec, roughness * roughness);
            float nDotH = max(dot(normal, halfVec), 0.0);
            float hDotV = max(dot(halfVec, viewVec), 0.0);
            float pdf = max(D * nDotH / (4.0 * hDotV), 0.0001); 

            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / max(float(numSamples) * pdf, 0.0001);

            float mipLevel = (roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel)); 
            
            prefilteredColor += textureLod(environmentMap, lightVec, mipLevel).rgb * nDotL;
            totalWeight += nDotL;
        }
    }

    prefilteredColor /= totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
}
