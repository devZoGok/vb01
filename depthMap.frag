#version 330 core

uniform float farPlane;
uniform bool point;
uniform vec3 lightPos;

in vec4 fragPos;

void main(){
	if(point){
		float dist = length(lightPos - fragPos.xyz);
		dist /= farPlane;
		gl_FragDepth = dist;
	}
}
