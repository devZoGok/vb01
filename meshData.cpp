#include "meshData.h"

using namespace std;

namespace vb01{
	void MeshData::ShapeKey::animate(float value, KeyframeChannel keyframeChannel){
		switch(keyframeChannel.type){
			case KeyframeChannel::SHAPE_KEY_MIN:
				this->minValue = value;
				break;
			case KeyframeChannel::SHAPE_KEY_VALUE:
				this->value = value;
				break;
			case KeyframeChannel::SHAPE_KEY_MAX:
				this->maxValue = value;
				break;
		}
	}

	MeshData::OldVertex* MeshData::toGlVerts(){
		int numVertices = 3 * numTris;
		OldVertex *vertData = new OldVertex[numVertices];

		for(int i = 0; i < numVertices; i++){
			vertData[i].pos = *vertices[i].pos;
			vertData[i].norm = *vertices[i].norm;
			vertData[i].tan = vertices[i].tan;
			vertData[i].biTan = vertices[i].biTan;
			vertData[i].uv = vertices[i].uv;

			if(weights){
				for(int j = 0; j < 4; j++){
					vertData[i].weights[j] = vertices[i].weights[j];
					vertData[i].boneIndices[j] = vertices[i].boneIndices[j];
				}

			}

			//if(shapeKeyOffsets) for(int j = 0; j < 100; j++) vertData[i].shapeKeyOffsets[j] = vertices[i].shapeKeyOffsets[j];
		}

		return vertData;
	}
}
