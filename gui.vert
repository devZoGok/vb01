#version 460 core

layout (location = 0) in vec3 aVert;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;
out int ID;

uniform vec2 screen;

struct GuiObjectData{
	float pos[3];
	bool texturingEnabled;
	int pastTexture[2], nextTexture[2];
	float diffuseColor[4];
};

layout(std430, binding = 0) readonly restrict buffer objSSBO {
	GuiObjectData guiObjData[];
};

void main(){
	int id = gl_InstanceID;
	vec3 pos = vec3(guiObjData[id].pos[0], guiObjData[id].pos[1], guiObjData[id].pos[2]);

	float x, y;
	x = (aVert.x + pos.x - screen.x * .5) / (screen.x * .5);
	y = (screen.y * .5 - (aVert.y + pos.y)) / (screen.y * .5);
	gl_Position = vec4(x, y, -(pos.z + aVert.z), 1);
	texCoords = aTexCoords;

	ID = id;
}
