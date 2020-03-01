#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform bool horizontal;
uniform sampler2D tex;

void main(){
	int amount=5;
	float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054,0.016216);
	vec2 texel=1.0/textureSize(tex,0);
	vec3 c=texture(tex,texCoords).rgb*weight[0];

	for(int i=1;i<amount;i++){
		vec2 offset=vec2(horizontal?texel.x:0,horizontal?0:texel.y);
		c+=texture(tex,texCoords+offset*i).rgb*weight[i];
		c+=texture(tex,texCoords-offset*i).rgb*weight[i];
	}
	FragColor=vec4(c,1);
}
