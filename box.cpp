#include"box.h"

namespace vb01{
	Box::Box(){
		float size=.1;
		float vertData[]={
			0,1,0,1,1,
			0,1,0,0,1,
			0,1,0,0,0,
			
			0,1,0,0,0,
			0,1,0,1,0,
			0,1,0,1,1,

			0,-1,0,1,1,
			0,-1,0,0,1,
			0,-1,0,0,0,
			
			0,-1,0,0,0,
			0,-1,0,1,0,
			0,-1,0,1,1,

			0,0,1,1,1,
			0,0,1,0,1,
			0,0,1,0,0,
			
			0,0,1,0,0,
			0,0,1,1,0,
			0,0,1,1,1,

			0,0,-1,1,1,
			0,0,-1,0,1,
			0,0,-1,0,0,
			
			0,0,-1,0,0,
			0,0,-1,1,0,
			0,0,-1,1,1,

			1,0,0,1,1,
			1,0,0,0,1,
			1,0,0,0,0,
			
			1,0,0,0,0,
			1,0,0,1,0,
			1,0,0,1,1,

			-1,0,0,1,1,
			-1,0,0,0,1,
			-1,0,0,0,0,
			
			-1,0,0,0,0,
			-1,0,0,1,0,
			-1,0,0,1,1
		};	
		Vector3 p[]={
			Vector3(size.x/2,size.y/2,size.z/2), 
			Vector3(-size.x/2,size.y/2,size.z/2), 
			Vector3(-size.x/2,size.y/2,-size.z/2),
			Vector3(-size.x/2,size.y/2,-size.z/2),

			Vector3(size.x/2,-size.y/2,size.z/2),
			Vector3(-size.x/2,-size.y/2,size.z/2),
			Vector3(-size.x/2,-size.y/2,-size.z/2),
			Vector3(-size.x/2,-size.y/2,-size.z/2)
		};
		int indices[]={
			
		};
	}
	void Box::update(){
		
	}
}
