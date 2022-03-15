#ifndef MESH_BASE_H
#define MESH_BASE_H

#include <string>

#include "vector.h"
#include "util.h"
#include "keyframeChannel.h"
#include "animatable.h"

namespace vb01{
		struct MeshData{
			struct ShapeKey : public Animatable{
				std::string name;
				float minValue, value, maxValue;
			
				void animate(float, KeyframeChannel);
			};
			
			struct Vertex{
				Vector3 pos, norm, tan, biTan;
				Vector2 uv;	
				float weights[4]{0, 0, 0, 0};
				int boneIndices[4]{-1, -1, -1, -1};
				Vector3 shapeKeyOffsets[100];
			};

			Vertex *vertices;
			std::string *vertexGroups = nullptr;
			ShapeKey *shapeKeys = nullptr;
			u32 *indices, VAO, VBO, EBO;
			int numTris, numVertexGroups = 0, numShapeKeys = 0;

			MeshData(){}
			MeshData(Vertex*, u32*, int, std::string *vg = nullptr, int = 0, ShapeKey *sk = nullptr, int = 0);
		};
}

#endif
