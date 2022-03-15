#include "box.h"

namespace vb01{
	Box::Box(Vector3 size){
		setSize(size);
		construct();
	}

	void Box::setSize(Vector3 size){
		this->size = size;

		Vector3 pos[] = {
			Vector3(size.x / 2, size.y / 2, size.z / 2),
			Vector3(-size.x / 2, size.y / 2, size.z / 2),
			Vector3(-size.x / 2, size.y / 2, -size.z / 2),
			Vector3(size.x / 2, size.y / 2, -size.z / 2),

			Vector3(size.x / 2, -size.y / 2, size.z / 2),
			Vector3(-size.x / 2, -size.y / 2, size.z / 2),
			Vector3(-size.x / 2, -size.y / 2, -size.z / 2),
			Vector3(size.x / 2, -size.y / 2, -size.z / 2)
		};

		Vector3 norm[] = {
			Vector3(0, 0, 1),
			Vector3(0, 1, 0),
			Vector3(1, 0, 0),

			Vector3(0, 0, -1),
			Vector3(0, -1, 0),
			Vector3(-1, 0, 0)
		};

		Vector2 tex[] = {
			Vector2(1, 1),
			Vector2(0, 1),
			Vector2(0, 0),
			Vector2(1, 0)
		};

		unsigned int data[] = {
			1, 3, 2, 4, 3, 0, 0, 3, 1,
			4, 3, 0, 1, 3, 2, 5, 3, 3,

			3, 0, 2, 6, 0, 0, 2, 0, 1,	
			3, 0, 3, 7, 0, 0, 6, 0, 2,

			4, 2, 2, 7, 2, 1, 3, 2, 0,
			3, 2, 0, 0, 2, 3, 4, 2, 1,

			6, 4, 2, 5, 4, 1, 1, 4, 0,
			1, 4, 0, 2, 4, 3, 6, 4, 2,

			2, 1, 2, 1, 1, 1, 0, 1, 0,
			0, 1, 0, 3, 1, 3, 2, 1, 2,

			5, 5, 2, 6, 5, 1, 4, 5, 3,
			7, 5, 0, 4, 5, 3, 6, 5, 1
		};

		int numTris = 12;
		u32 *indices = new u32[3 * numTris];
		MeshData::Vertex *vertices = new MeshData::Vertex[3 * numTris];

		for(int i = 0; i < 3 * numTris; i++){
				MeshData::Vertex v;
			v.pos = pos[data[3 * i]];
			v.norm = norm[data[3 * i + 1]];
			v.uv = tex[data[3 * i + 2]];
			vertices[i] = v;
			indices[i] = i;
		}

		meshBase = MeshData(vertices, indices, 3 * numTris);
	}
}
