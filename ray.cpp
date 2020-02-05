#include<algorithm>
#include"ray.h"
#include"mesh.h"
#include"node.h"

using namespace std;

namespace vb01{
	void retrieveCollisions(Vector3 rayPos, Vector3 rayDir,Node *node,std::vector<CollisionResult> &results, const float rayLength){
		Vector3 pos=node->getWorldPosition();
		Quaternion rot=node->getWorldOrientation();
		for(Mesh *m : node->getMeshes()){
			const int numVerts=m->getNumVerts();
			Mesh::Vertex *vertices=m->getVerts();
			u32 *indices=m->getIndices();

			for(int i=0;i<numVerts/3;i++){
				Vector3 pointA=pos+rot*vertices[indices[i*3]].pos,pointB=pos+rot*vertices[indices[i*3+1]].pos,pointC=pos+rot*vertices[indices[i*3+2]].pos;
				Vector3 hypVec=pointA-rayPos;
				Vector3 perpVec=(pointB-pointA).cross(pointC-pointA);
				float a1=hypVec.norm().getAngleBetween(perpVec.norm());
				if(a1>PI/2){
					a1=PI-a1;
					perpVec=-perpVec;
				}	 
				float perpLine=hypVec.getLength()*cos(a1);
		   		float a2=perpVec.norm().getAngleBetween(rayDir.norm());
				if(a2<=PI/2){
					float distance=perpLine/cos(a2);
					if((distance<=rayLength&&rayLength!=.0)||rayLength==.0){
						Vector3 contactPoint=rayPos+rayDir.norm()*distance;
						float angleA=(pointB-pointA).norm().getAngleBetween((pointC-pointA).norm());
						float angleB=(pointA-pointB).norm().getAngleBetween((pointC-pointB).norm());
						float angleC=(pointB-pointC).norm().getAngleBetween((pointA-pointC).norm());
						Vector3 bisecAVec=((pointB-pointA)+(pointC-pointA));
						Vector3 bisecBVec=((pointA-pointB)+(pointC-pointB));
						Vector3 bisecCVec=((pointB-pointC)+(pointA-pointC));
						bool withinBisecA=(contactPoint-pointA).norm().getAngleBetween(bisecAVec.norm())<=angleA/2;
						bool withinBisecB=(contactPoint-pointB).norm().getAngleBetween(bisecBVec.norm())<=angleB/2;
						//bool withinBisecC=(contactPoint-pointC).norm().getAngleBetween(bisecCVec.norm())<=angleC/2;
						if(withinBisecA&&withinBisecB){
							CollisionResult result;
							result.pos=contactPoint;
							result.distance=distance;
							result.mesh=m;
							results.push_back(result);
						}
					}
				}
			}
		}
		for(Node *c : node->getChildren())
			retrieveCollisions(rayPos,rayDir,c,results,rayLength);
	}
        
	void sortResults(std::vector<CollisionResult> &results){
		if(!results.empty())
			for(int i=0;i<results.size();i++){
				for(int i2=i;i2<results.size();i2++)
					if(results[i].distance>results[i2].distance)
						swap(results[i],results[i2]);
			}
	}
}
