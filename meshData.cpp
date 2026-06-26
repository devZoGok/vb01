#include "meshData.h"
#include "root.h"

#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <ext.hpp>

using namespace std;

namespace vb01{
	//TODO allow to init meshes to be drawn statically or dynamically
	MeshData::MeshData(
				Vector3 *pos,
				float **w,
				int **bi,
				Vector3 **sko,
				int np,
				Vector3 *norm,
				Vertex *vert,
				u32 *ind,
				int nt,
				std::string an,
				std::string *vg,
				int nvg,
				std::string fsn,
				ShapeKey *sk,
				int nsk
			) : 
				positions(pos),
				weights(w),
				boneIndices(bi),
				shapeKeyOffsets(sko),
				numPos(np),
				normals(norm),
				vertices(vert), 
				indices(ind), 
				numTris(nt), 
				vertexGroups(vg), 
				numVertexGroups(nvg), 
				fullSkeletonName(fsn), 
				shapeKeys(sk), 
				numShapeKeys(nsk)
   	{}

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

	//TODO replace number of max bone influences literal
	vector<MeshData::GpuVertex> MeshData::toGpuVerts(){
		int numVertices = 3 * numTris;
		vector<GpuVertex> vertData;

		for(int i = 0; i < numVertices; i++){
			MeshData::GpuVertex gpuVert;
			gpuVert.pos = *vertices[i].pos;
			gpuVert.norm = *vertices[i].norm;
			gpuVert.tan = vertices[i].tan;
			gpuVert.biTan = vertices[i].biTan;
			gpuVert.uv = vertices[i].uv;

			if(weights){
				for(int j = 0; j < 4; j++){
					gpuVert.weights[j] = vertices[i].weights[j];
					gpuVert.boneIndices[j] = vertices[i].boneIndices[j];
				}
			}
			/*

			*/
			if(shapeKeyOffsets){
				const int MAX_NUM_SHAPE_KEYS = Root::getSingleton()->getMaxNumShapeKeys();

				for(int j = 0; j < MAX_NUM_SHAPE_KEYS; j++) 
					gpuVert.shapeKeyOffsets[j] = vertices[i].shapeKeyOffsets[j];
			}

			vertData.push_back(gpuVert);
		}

		return vertData;
	}

	bool MeshData::operator==(MeshData &meshData){
		bool samePos = (positions == meshData.positions);
		bool sameNorm = (normals == meshData.normals);
		bool sameSkOffsets = (shapeKeyOffsets == meshData.shapeKeyOffsets);
		return samePos && sameNorm && sameSkOffsets;
	}
}
