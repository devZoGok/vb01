#version 460 core

layout (location = 0) in vec3 aVert;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out int ID;

uniform vec2 screen;

struct GuiObjectData{
	float pos[3], scale[3], size[2];
	bool texturingEnabled;
	int glyphTexture[2], pastTexture[2], nextTexture[2];
	float diffuseColor[4];
};

layout(std430, binding = 0) readonly restrict buffer objSSBO {
	GuiObjectData guiObjData[];
};

void main(){
	int id = gl_InstanceID;
	vec3 scale = vec3(guiObjData[id].scale[0], guiObjData[id].scale[1], guiObjData[id].scale[2]);
	vec3 pos = vec3(guiObjData[id].pos[0], guiObjData[id].pos[1], guiObjData[id].pos[2]) * scale;
	vec2 size = vec2(guiObjData[id].size[0], guiObjData[id].size[1]);
	vec3 vertPos = vec3(aVert.xy * scale.xy * size.xy, aVert.z);

	float x, y;
	x = (vertPos.x + pos.x - screen.x * .5) / (screen.x * .5);
	y = (screen.y * .5 - (vertPos.y + pos.y)) / (screen.y * .5);
	gl_Position = vec4(x, y, -(pos.z + vertPos.z), 1);
	texCoords = vec2(aTexCoords.x * size.x, (1.0 - aTexCoords.y) * size.y);

	ID = id;
}
