#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D frag;
uniform sampler2D bright;
uniform bool bloom;

void main(){
	vec4 hdrColor=texture(frag,texCoords);
	vec4 brightColor=texture(bright,texCoords);

	if(bloom){
	float blurKernel[9]=float[](
		1.0/16,2.0/16,1.0/16,
		2.0/16,4.0/16,1.0/16,
		1.0/16,2.0/16,1.0/16);
	vec2 texel=1.0/textureSize(bright,0);

	vec3 sum=vec3(0);
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			vec3 adjSample=texture(frag,texCoords+vec2(texel.x*(j-1),texel.y*(i-1))).rgb;
			adjSample*=blurKernel[i*3+j];
			sum+=adjSample;
		}
	}
	hdrColor.rgb=sum;
}

	//hdrColor=brightColor;
	float gamma=1,exposure=1;

	//Reinhardt
	//c.rgb=c.rgb/(c.rgb+vec3(1.));

	//Exp
	hdrColor.rgb=vec3(1.)-exp(-hdrColor.rgb*exposure);

	hdrColor.rgb=pow(hdrColor.rgb,vec3(1/gamma));
	FragColor=hdrColor;
}
