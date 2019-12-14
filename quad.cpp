#include"quad.h"

namespace vb01{
	Quad::Quad(Vector3 size,bool spatial,bool spatialToScreen) : Mesh(){
		this->spatial=spatial;
		this->staticVerts=false;
		this->spatialToScreen=spatialToScreen;
		
		int numPolyVerts=3;
		numTris=2;
		indices=new unsigned int[numTris*numPolyVerts];
		vertices=new Vertex[numTris*numPolyVerts];

		setSize(size);

		Mesh::construct();
	}
	
	void Quad::setSize(Vector3 size){
		this->size=size;

		Vertex v1,v2,v3,v4,v5,v6;
		if(spatial){
			v1.pos=Vector3(size.x/2,size.y/2,0);
			v2.pos=Vector3(-size.x/2,size.y/2,0);
			v3.pos=Vector3(-size.x/2,-size.y/2,0);
			v4.pos=Vector3(-size.x/2,-size.y/2,0);
			v5.pos=Vector3(size.x/2,-size.y/2,0);
			v6.pos=Vector3(size.x/2,size.y/2,0);
		}
		else{
			v1.pos=Vector3(size.x,0,0);
			v2.pos=Vector3(0,0,0);
			v3.pos=Vector3(0,size.y,0);
			v4.pos=Vector3(0,size.y,0);
			v5.pos=Vector3(size.x,size.y,0);
			v6.pos=Vector3(size.x,0,0);
		}
		v1.norm=Vector3(0,0,-1);
		v1.texCoords=Vector2(1,1);

		v2.norm=Vector3(0,0,-1);
		v2.texCoords=Vector2(0,1);

		v3.norm=Vector3(0,0,-1);
		v3.texCoords=Vector2(0,0);

		v4.norm=Vector3(0,0,-1);
		v4.texCoords=Vector2(0,0);

		v5.norm=Vector3(0,0,-1);
		v5.texCoords=Vector2(1,0);

		v6.norm=Vector3(0,0,-1);
		v6.texCoords=Vector2(1,1);
		
		indices[0]=0;
		indices[1]=1;
		indices[2]=2;
		indices[3]=3;
		indices[4]=4;
		indices[5]=5;

		vertices[0]=v1;
		vertices[1]=v2;
		vertices[2]=v3;
		vertices[3]=v4;
		vertices[4]=v5;
		vertices[5]=v6;
	}

}
