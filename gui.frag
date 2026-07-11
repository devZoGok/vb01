#version 460 core

in vec2 texCoords;
in flat int ID;

out vec4 FragColor;

struct GuiObjectData{
	float pos[3], scale[3], size[2];
	bool texturingEnabled;
	int glyphTexture[2], pastTexture[2], nextTexture[2];
	float diffuseColor[4];
};

layout(std430, binding = 0) readonly restrict buffer objSSBO {
	GuiObjectData guiObjData[];
};

uniform sampler2DArray textureSamplers[128];

void main(){
	int id = ID;
	vec4 c;

	if(guiObjData[id].texturingEnabled)
		c = texture(textureSamplers[guiObjData[id].pastTexture[0]], vec3(texCoords.xy, guiObjData[id].pastTexture[1]));
	else
		c = vec4(guiObjData[id].diffuseColor[0], guiObjData[id].diffuseColor[1], guiObjData[id].diffuseColor[2], guiObjData[id].diffuseColor[3]);

	bool glyph = !(guiObjData[id].glyphTexture[0] == -1 || guiObjData[id].glyphTexture[0] == -1);

	if(glyph){
		float alpha = texture(textureSamplers[guiObjData[id].glyphTexture[0]], vec3(texCoords.xy, guiObjData[id].glyphTexture[1])).r; 
		c.w *= alpha;
	}

	FragColor = c;
}
