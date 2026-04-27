#version 330 core

/*
*/
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 tCoords;
}gs_in[];

out vec4 FragPos;
out vec2 texCoords;

uniform mat4 shadowMat[6];
uniform bool environment;

void main(){
/*
	if(environment)
		for(int i = 0; i < 6; i++){
			gl_Layer = i;
			for(int j = 0; j < 3; j++){
				FragPos = gl_in[j].gl_Position;
				gl_Position = shadowMat[i] * FragPos;
				EmitVertex();
			}
			EndPrimitive();
		}
*/
gl_Position = gl_in[0].gl_Position;
texCoords = gs_in[0].tCoords;
EmitVertex();
gl_Position = gl_in[1].gl_Position;
texCoords = gs_in[1].tCoords;
EmitVertex();
gl_Position = gl_in[2].gl_Position;
texCoords = gs_in[2].tCoords;
EmitVertex();
EndPrimitive();
}
