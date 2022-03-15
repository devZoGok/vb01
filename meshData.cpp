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

	MeshData::MeshData(Vertex *vertices, u32 *indices, int numTris, string *vertexGroups, int numVertexGroups, ShapeKey *shapeKeys, int numShapeKeys){
		this->vertices = vertices;
		this->indices = indices;
		this->numTris = numTris;
		this->vertexGroups = vertexGroups;
		this->numVertexGroups = numVertexGroups;
		this->shapeKeys = shapeKeys;
		this->numShapeKeys = numShapeKeys;
	}
}
