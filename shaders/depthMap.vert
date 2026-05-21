#version 330 core 

layout (location = 0) in vec3 aPos;

uniform mat4 model, lightMat;
uniform bool point;

void main(){
	if(point)
		gl_Position = model * vec4(aPos, 1);
	else
		gl_Position = lightMat * model * vec4(aPos, 1);	
}
