#include "quad.h"

#include <algorithm>

namespace vb01{
	using namespace std;

	Quad::Quad(Vector3 size, bool spatial, int numVertDiv, int numHorDiv){
		this->spatial = spatial;
		this->numVertDiv = numVertDiv;
		this->numHorDiv = numHorDiv;

		setSize(size);
	}
	
	void Quad::setSize(Vector3 size){
		this->size = size;

		const int numHorQuads = numVertDiv + 1, numVertQuads = numHorDiv + 1;
		const int numVertPos = (numHorQuads + 1) * (numVertQuads + 1), numTris = 2 * numHorQuads * numVertQuads, numVerts = 3 * numTris;
		Vector2 subQuadSize = Vector2(size.x / numHorQuads, size.y / numVertQuads);

		MeshData::Vertex *vertices = new MeshData::Vertex[numVerts];

		if(!meshBase.positions){
			meshBase.positions = new Vector3[numVertPos];
			meshBase.normals = new Vector3(Vector3::VEC_J);
			meshBase.indices = new u32[numVerts];

			for(int i = 0; i < numVerts; i++)
				meshBase.indices[i] = i;
		}

		Vector3 *faceVertPos = meshBase.positions;
		Vector3 *norm = meshBase.normals;
		u32 *indices = meshBase.indices;

		for(int i = 0; i < numHorQuads; i++){
			for(int j = 0; j < numVertQuads; j++){
				int vertPosId = i * (numHorQuads + 1) + j;
				int vertPosId2 = (i + 1) * (numHorQuads + 1) + j;
				Vector2 offset = Vector2(subQuadSize.x, subQuadSize.y);

				faceVertPos[vertPosId] = Vector3(-.5 * size.x + offset.x * i, 0, -.5 * size.y + offset.y * j);
				faceVertPos[vertPosId + 1] = Vector3(-.5 * size.x + offset.x * i, 0, -.5 * size.y + offset.y * (j + 1));
				faceVertPos[vertPosId2] = Vector3(-.5 * size.x + offset.x * (i + 1),  0, -.5 * size.y + offset.y * j);
				faceVertPos[vertPosId2 + 1] = Vector3(-.5 * size.x + offset.x * (i + 1),  0, -.5 * size.y + offset.y * (j + 1));

				if(!spatial){
					int ids[4]{vertPosId, vertPosId + 1, vertPosId2, vertPosId2 + 1};

					swap(faceVertPos[ids[0]].z, faceVertPos[ids[1]].z);
					swap(faceVertPos[ids[2]].z, faceVertPos[ids[3]].z);

					for(int k = 0; k < 4; k++){
						faceVertPos[ids[k]].y = -faceVertPos[ids[k]].z;
						faceVertPos[ids[k]].z = 0;
						faceVertPos[ids[k]] += .5 * Vector3(size.x, size.y, 0);
					}
				}

				Vector2 faceVertUv[]{
					Vector2(float(i) / numHorQuads, 1 - float(j) / numVertQuads),
					Vector2(float(i) / numHorQuads, 1 - float(j + 1) / numVertQuads),
					Vector2(float(i + 1) / numHorQuads, 1 - float(j + 1) / numVertQuads),
					Vector2(float(i + 1) / numHorQuads, 1 - float(j + 1) / numVertQuads),
					Vector2(float(i + 1) / numHorQuads, 1 - float(j) / numVertQuads),
					Vector2(float(i) / numHorQuads, 1 - float(j) / numVertQuads),
				};

				int quadId = 6 * (i * numHorQuads + j);
				int vertPosIds[6]{vertPosId, vertPosId + 1, vertPosId2 + 1, vertPosId2 + 1, vertPosId2, vertPosId};

				for(int k = 0; k < 6; k++){
					MeshData::Vertex v;
					v.pos = &faceVertPos[vertPosIds[k]];
					v.uv = faceVertUv[k];
					v.tan = Vector3::VEC_I;
					v.norm = norm;
					v.biTan = Vector3::VEC_K;
					vertices[quadId + k] = v;
				}
			}
		}

		meshBase = MeshData(faceVertPos, nullptr, nullptr, nullptr, 6, norm, vertices, indices, numTris);
	}
}
