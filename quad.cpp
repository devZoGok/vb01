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

	//TODO improve functionality for GUI quads 
	Vector2 Quad::getSubquadIds(int numVertDiv, int numHorDiv, Vector3 pos, Vector3 size){
		const int numHorQuads = numVertDiv + 1, numVertQuads = numHorDiv + 1;
		Vector2 subQuadSize = Vector2(size.x / numHorQuads, size.z / numVertQuads);

		return Vector2(int((.5 * size.x + pos.x) / subQuadSize.x), int((.5 * size.z + pos.z) / subQuadSize.y));
	}

	Vector3 Quad::getSubquadCorner(int xId, int yId, int numVertDiv, int numHorDiv, bool top, bool right){
		const int numHorQuads = numVertDiv + 1, numVertQuads = numHorDiv + 1;
		int vertPosId = 6 * xId * numVertQuads + 6 * yId;

		if(top && !right) return meshBase.positions[vertPosId];
		else if(top && right) return meshBase.positions[vertPosId + 4];
		else if(!top && !right) return meshBase.positions[vertPosId + 1];
		else if(!top && right) return meshBase.positions[vertPosId + 3];
	}

	//TODO implement checks to clamp the number of points to be within the mesh
	vector<Vector3> Quad::getVertPosInQuad(int numVertDiv, int numHorDiv, Vector3 meshSize, Vector3 quadPos, Vector3 quadSize){
		Vector2 topLeftSubquad = getSubquadIds(numVertDiv, numHorDiv, quadPos - .5 * quadSize, meshSize);
		Vector3 bottRightCorner = getSubquadCorner(topLeftSubquad.x, topLeftSubquad.y, numVertDiv, numHorDiv, false, true);

		const int numHorQuads = numVertDiv + 1, numVertQuads = numHorDiv + 1;
		Vector2 subquadSize = Vector2(meshSize.x / numHorQuads, meshSize.z / numVertQuads);

		int numHorPoints = int(fabs(quadPos.x + .5 * quadSize.x - bottRightCorner.x) / subquadSize.x);
		int numVertPoints = int(fabs(quadPos.z + .5 * quadSize.z - bottRightCorner.z) / subquadSize.y);

		vector<Vector3> positions;

		for(int i = 0; i < numVertPoints; i++)
			for(int j = 0; j < numHorPoints; j++){
				Vector3 corner = getSubquadCorner(topLeftSubquad.x + j, topLeftSubquad.y + i, numVertDiv, numHorDiv, false, true);
				positions.push_back(corner);
			}

		return positions;
	}

	float Quad::getHeightInSubquad(int numVertDiv, int numHorDiv, Vector3 meshSize, Vector3 pos){
		Vector2 subquadIds = getSubquadIds(numVertDiv, numHorDiv, pos, meshSize);
		Vector3 topLeft = getSubquadCorner(subquadIds.x, subquadIds.y, numVertDiv, numHorDiv, true, false);
		Vector3 topRight = getSubquadCorner(subquadIds.x, subquadIds.y, numVertDiv, numHorDiv, true, true);
		Vector3 bottLeft = getSubquadCorner(subquadIds.x, subquadIds.y, numVertDiv, numHorDiv, false, false);
		Vector3 bottRight = getSubquadCorner(subquadIds.x, subquadIds.y, numVertDiv, numHorDiv, false, true);

		Vector3 v1 = (bottLeft - topLeft), v2 = (bottRight - topLeft);
		v1.y = 0, v2.y = 0;
		float angle = v1.norm().getAngleBetween(v2.norm());

		float hypZ = topLeft.z + (topRight.x - topLeft.x) / tan(angle);
		bool leftTri = pos.z > hypZ;

		float xRatio = (pos.x - topLeft.x) / (topRight.x - topLeft.x);
		float zRatio = (pos.z - topLeft.z) / v1.z;
		float height;

		if(leftTri) height = (topLeft + (bottRight - bottLeft) * xRatio + (bottLeft - topLeft) * zRatio).y;
		else height = (topLeft + (topRight - topLeft) * xRatio + (bottRight - topRight) * zRatio).y;

		return height;
	}
}
