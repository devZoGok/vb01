#include "meshData.h"

#include "glad.h"
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
   	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);	

		u32 size = sizeof(MeshData::GpuVertex);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		MeshData::GpuVertex *glVertData = toGpuVerts();
		glBufferData(GL_ARRAY_BUFFER, 3 * numTris * size, glVertData, GL_DYNAMIC_DRAW);
		delete glVertData;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numTris * sizeof(u32), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, norm)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, uv)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, tan)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, biTan)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, weights)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, boneIndices)));
		glEnableVertexAttribArray(6);

		for(int i = 0; i < numShapeKeys; i++){
			glVertexAttribPointer(7 + i, 3, GL_FLOAT, GL_FALSE, size, (void*)(offsetof(MeshData::GpuVertex, shapeKeyOffsets) + 3 * i * sizeof(float)));
			glEnableVertexAttribArray(7 + i);
		}
	}

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

	MeshData::GpuVertex* MeshData::toGpuVerts(){
		int numVertices = 3 * numTris;
		GpuVertex *vertData = new GpuVertex[numVertices];

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

			if(shapeKeyOffsets)
				for(int j = 0; j < 100; j++) 
					vertData[i].shapeKeyOffsets[j] = vertices[i].shapeKeyOffsets[j];
		}

		return vertData;
	}
}
