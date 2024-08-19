#include "quad.h"

namespace vb01{
	Quad::Quad(Vector3 size, bool spatial, int numVertDiv, int numHorDiv){
		this->spatial = spatial;
		this->numVertDiv = numVertDiv;
		this->numHorDiv = numHorDiv;

		setSize(size);
		Mesh::construct();
	}
	
	void Quad::setSize(Vector3 size){
		this->size = size;

		const int numHorQuads = numVertDiv + 1, numVertQuads = numHorDiv + 1;
		const int numTris = 2 * numHorQuads * numVertQuads, numVerts = 3 * numTris;
		Vector2 subQuadSize = Vector2(size.x / numHorQuads, size.y / numVertQuads);

		MeshData::Vertex *vertices = new MeshData::Vertex[numVerts];
		Vector3 *faceVertPos = new Vector3[6];

		Vector3 *norm = new Vector3(Vector3::VEC_J);

		for(int i = 0; i < numHorQuads; i++){
			for(int j = 0; j < numVertQuads; j++){
				int id = i * 6 * numHorQuads + j * 6;
				Vector2 offset = Vector2(subQuadSize.x, subQuadSize.y);

				faceVertPos[0] = Vector3(-.5 * size.x + offset.x * j, 0, -.5 * size.y + offset.y * i);
				faceVertPos[1] = Vector3(-.5 * size.x + offset.x * (j + 1),  0, -.5 * size.y + offset.y * (i + 1));
				faceVertPos[2] = Vector3(-.5 * size.x + offset.x * (j + 1),  0, -.5 * size.y + offset.y * i);
				faceVertPos[3] = Vector3(-.5 * size.x + offset.x * j, 0, -.5 * size.y + offset.y * i);
				faceVertPos[4] = Vector3(-.5 * size.x + offset.x * j, 0, -.5 * size.y + offset.y * (i + 1));
				faceVertPos[5] = Vector3(-.5 * size.x + offset.x * (j + 1),  0, -.5 * size.y + offset.y * (i + 1));

				Vector2 faceVertUv[]{
					Vector2(float(j) / numHorQuads, float(i) / numVertQuads),
					Vector2(float(j + 1) / numHorQuads, float(i + 1) / numVertQuads),
					Vector2(float(j + 1) / numHorQuads, float(i) / numVertQuads),
					Vector2(float(j) / numHorQuads, float(i) / numVertQuads),
					Vector2(float(j) / numHorQuads, float(i + 1) / numVertQuads),
					Vector2(float(j + 1) / numHorQuads, float(i + 1) / numVertQuads),
				};

				for(int k = 0; k < 6; k++){
					if(!spatial){
						faceVertPos[k].y = -faceVertPos[k].z;
						faceVertPos[k].z = 0;
						faceVertPos[k] += .5 * Vector3(size.x, size.y, 0);
					}

					MeshData::Vertex v;
					v.pos = &faceVertPos[k];
					v.uv = faceVertUv[k];
					v.tan = Vector3::VEC_I;
					v.norm = norm;
					v.biTan = Vector3::VEC_K;
					vertices[id + k] = v;
				}
			}
		}

		u32 *indices = new u32[numVerts];

		for(int i = 0; i < numVerts; i++)
			indices[i] = i;

		meshBase = MeshData((Vector3*)faceVertPos, nullptr, nullptr, nullptr, 6, nullptr, vertices, indices, numTris);
	}
}
