#include"box.h"

namespace vb01{
	Box::Box(Vector3 size){
		Vector3 pos[]={
			Vector3(size.x/2,size.y/2,size.z/2),
			Vector3(-size.x/2,size.y/2,size.z/2),
			Vector3(-size.x/2,size.y/2,-size.z/2),
			Vector3(size.x/2,size.y/2,-size.z/2),

			Vector3(size.x/2,-size.y/2,size.z/2),
			Vector3(-size.x/2,-size.y/2,size.z/2),
			Vector3(-size.x/2,-size.y/2,-size.z/2),
			Vector3(size.x/2,-size.y/2,-size.z/2)
		};
		Vector3 norm[]={
			Vector3(0,0,1),
			Vector3(0,1,0),
			Vector3(1,0,0),

			Vector3(0,0,-1),
			Vector3(0,-1,0),
			Vector3(-1,0,0)
		};
		Vector2 tex[]={
			Vector2(1,1),
			Vector2(0,1),
			Vector2(0,0),
			Vector2(1,0)
		};
		unsigned int data[]={
			0,3,1, 4,3,0, 1,3,2,	
			5,3,3, 1,3,2, 4,3,0,

			2,0,1, 6,0,0, 3,0,2,	
			6,0,2, 7,0,0, 3,0,3,

			3,2,0, 7,2,1, 4,2,2,
			4,2,2, 0,2,3, 3,2,0,

			1,5,0, 5,5,1, 6,5,2,
			6,5,2, 2,5,3, 1,5,0,

			0,1,0, 1,1,1, 2,1,2,
			2,1,2, 3,1,3, 0,1,0,

			4,4,3, 6,4,1, 5,4,2,
			6,4,1, 4,4,3, 7,4,0
		};
		numTris=12;
		indices=new unsigned int[3*numTris];
		vertices=new Vertex[3*numTris];
		for(int i=0;i<3*numTris;i++){
			Vertex v;
			v.pos=pos[data[3*i]];
			v.norm=norm[data[3*i+1]];
			v.texCoords=tex[data[3*i+2]];
			vertices[i]=v;
			indices[i]=i;	
		}
		construct();
	}
}
