#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D frag, bright;
uniform bool hdr, bloom;
uniform float exposure, gamma;

void main(){
	vec4 hdrColor = texture(frag, texCoords);
	vec4 brightColor = texture(bright, texCoords);
	vec2 texel = 1.0 / textureSize(bright, 0);

	if(bloom)
		hdrColor += brightColor;

	if(hdr){
		//Reinhardt
		//c.rgb = c.rgb / (c.rgb + vec3(1.));

		//Exp
		hdrColor.rgb = vec3(1.) - exp(-hdrColor.rgb * exposure);
	}

	hdrColor.rgb = pow(hdrColor.rgb, vec3(1 / gamma));
	FragColor = hdrColor;
}
