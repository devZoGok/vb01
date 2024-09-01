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
			
				ShapeKey(std::string n, float v, float mnv, float mxv) : name(n), value(v), minValue(mnv), maxValue(mxv), Animatable(Animatable::SHAPE_KEY, n){}
				ShapeKey() : Animatable(Animatable::SHAPE_KEY){}
				void animate(float, KeyframeChannel);
			};
			
			struct OldVertex{
				Vector3 pos, norm, tan, biTan;
				Vector2 uv;	
				float weights[4]{0, 0, 0, 0};
				int boneIndices[4]{-1, -1, -1, -1};
				Vector3 shapeKeyOffsets[100];
			};

			struct Vertex{
				Vector3 *pos = nullptr, *norm = nullptr, tan, biTan;
				Vector2 uv;	
				float *weights = nullptr;
				int *boneIndices = nullptr;
				Vector3 *shapeKeyOffsets = nullptr;
			};

			OldVertex* toGlVerts();

			Vector3 *positions = nullptr, *normals = nullptr, **shapeKeyOffsets = nullptr;
			float **weights = nullptr;
			int **boneIndices = nullptr;
			Vertex *vertices;
			std::string *vertexGroups = nullptr;
			ShapeKey *shapeKeys = nullptr;
			u32 *indices, VAO, VBO, EBO;
			int numPos, numTris, numVertexGroups = 0, numShapeKeys = 0;
			std::string attachableName = "", fullSkeletonName = "";

			MeshData(){}
			MeshData(Vector3 *pos, float **w, int **bi, Vector3 **sko, int np, Vector3 *norm, Vertex *vert, u32 *ind, int nt, std::string an = "", std::string *vg = nullptr, int nvg = 0, std::string fsn = "", ShapeKey *sk = nullptr, int nsk = 0) : 
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
		};
}

#endif
