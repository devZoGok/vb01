#include"particleEmitter.h"
#include"shader.h"
#include<algorithm>
#include<glad.h>
#include<glfw3.h>
#include<cstdlib>
#include<ext.hpp>
#include"material.h"
#include"node.h"
#include"root.h"
#include"camera.h"

using namespace glm;
using namespace std;

namespace vb01{
	int n=0;

	ParticleEmitter::ParticleEmitter(int numParticles){
		this->numParticles=numParticles;

		particles=new Particle*[numParticles];

		Vertex v1,v2,v3,v4,v5,v6;
		Vector2 size=Vector2(.5,.5);
		v1.pos=Vector3(size.x/2,size.y/2,0);
		v1.texCoords=Vector2(1,1);

		v2.pos=Vector3(-size.x/2,size.y/2,0);
		v2.texCoords=Vector2(0,1);

		v3.pos=Vector3(-size.x/2,-size.y/2,0);
		v3.texCoords=Vector2(0,0);

		v4.pos=Vector3(-size.x/2,-size.y/2,0);
		v4.texCoords=Vector2(0,0);

		v5.pos=Vector3(size.x/2,-size.y/2,0);
		v5.texCoords=Vector2(1,0);

		v6.pos=Vector3(size.x/2,size.y/2,0);
		v6.texCoords=Vector2(1,1);
		
		ParticleEmitter::Vertex vertices[]={v1,v2,v3,v4,v5,v6};
		unsigned int indices[]={0,1,2,3,4,5};

		u32 MBO;
		mat4 *matrices=new mat4[numParticles];

		for(int i=0;i<numParticles;i++){
			Particle *p=new Particle;
			p->dir=direction;
			particles[i]=p;
			matrices[i]=mat4(1.f);
		}

		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,6*sizeof(Vertex),vertices,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(unsigned int),indices,GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,texCoords)));
		glEnableVertexAttribArray(1);
		glGenBuffers(1,&MBO);
		glBindBuffer(GL_ARRAY_BUFFER,MBO);
		glBufferData(GL_ARRAY_BUFFER,numParticles*sizeof(mat4),&matrices[0],GL_STATIC_DRAW);
		glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,sizeof(mat4),(void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(mat4),(void*)(1*sizeof(vec4)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE,sizeof(mat4),(void*)(2*sizeof(vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,sizeof(mat4),(void*)(3*sizeof(vec4)));
		glEnableVertexAttribArray(5);

		glVertexAttribDivisor(2,1);
		glVertexAttribDivisor(3,1);
		glVertexAttribDivisor(4,1);
		glVertexAttribDivisor(5,1);
	}

	ParticleEmitter::~ParticleEmitter(){
		glDeleteVertexArrays(1,&VAO);	
		glDeleteBuffers(1,&VBO);	

		delete material;

		delete[] particles;
	}

	void ParticleEmitter::makeHeap(int offset){
		bool heap=false;
		while(!heap){
			bool end=true;
			for(int i=0;2*i+1+offset<numParticles;i++){
				if(2*i+2+offset<numParticles&&(particles[i+offset]->d<particles[2*i+1+offset]->d||particles[i+offset]->d<particles[2*i+2+offset]->d)){
					bool leftChild=particles[2*i+1+offset]->d>particles[2*i+2+offset]->d;
					swap(particles[i+offset],leftChild?particles[2*i+1+offset]:particles[2*i+2+offset]);
					n++;
				}
				else if(2*i+2+offset==numParticles&&particles[i+offset]->d<particles[2*i+1+offset]->d){
					swap(particles[i+offset],particles[2*i+1+offset]);
					n++;
				}
			}
			for(int i=0;2*i+1<numParticles;i++){
				if(2*i+2+offset<numParticles&&(particles[i+offset]->d<particles[2*i+1+offset]->d||particles[i+offset]->d<particles[2*i+2+offset]->d))
					end=false;
				else if(2*i+2+offset==numParticles&&particles[i+offset]->d<particles[2*i+1+offset]->d)
					end=false;
			}
			if(end)
				heap=true;
		}
	}
	
	void ParticleEmitter::heapSort(){
		n=0;
		for(int i=0;i<numParticles;i++)
			makeHeap(i);
		cout<<n<<endl;
	}
	
	void ParticleEmitter::update(){
		Root *root=Root::getSingleton();
		Camera *cam=root->getCamera();
		float fov=cam->getFov(),width=root->getWidth(),height=root->getHeight();
		float nearPlane=cam->getNearPlane(),farPlane=cam->getFarPlane();
		Vector3 pos=node->getPosition();
		Vector3 camDir=cam->getDirection(),up=cam->getUp(),left=cam->getLeft(),camPos=cam->getPosition();

		mat4 view=lookAt(vec3(camPos.x,camPos.y,camPos.z),vec3(camPos.x+camDir.x,camPos.y+camDir.y,camPos.z+camDir.z),vec3(up.x,up.y,up.z));
		mat4 proj=perspective(radians(fov),width/height,nearPlane,farPlane);

		material->update();	
		Shader *shader=material->getShader();
		shader->setMat4(view,"view");
		shader->setMat4(proj,"proj");
		shader->setVec4(startColor,"startColor");
		shader->setVec4(endColor,"endColor");
		shader->setVec2(startSize,"startSize");
		shader->setVec2(endSize,"endSize");

		Vector3 nodePos=node->getWorldTransform().position;
		Vector3 nodeDir=node->getLocalAxis(2);
		Vector3 nodeUp=node->getLocalAxis(1);
		Vector3 nodeLeft=node->getLocalAxis(0);
		for(int i=0;i<numParticles;i++){
			Vector3 dir=particles[i]->dir;
			if(getTime()-particles[i]->time>=particles[i]->timeToLive){
				particles[i]->time=getTime();
				float factor=(float)(rand()%100)/100;
				float s1=float(rand()%(int)spread)/180*PI,s2=float(rand()%360)/180*PI;
				Vector3 ra1=(Vector3(0,1,0).cross(dir)).norm(),ra2=dir.norm();
				if(ra1==Vector3::VEC_ZERO) ra1=Vector3(1,0,0);
				dir=Quaternion(s1,ra1)*dir;
				dir=Quaternion(s2,ra2)*dir;
				particles[i]->timeToLive=s64(1000*((highLife-lowLife)*factor+lowLife));
				//particles[i].mat=translate(mat4(1.f),vec3(nodePos.x,nodePos.y,nodePos.z));
				particles[i]->trans=nodePos;
			}
			//dir=nodeLeft*dir.x+nodeUp*dir.y+nodeDir*dir.z;
			particles[i]->dir=dir.norm()*direction.getLengthSq()+gravity;
			float lifePercentage=(float)(getTime()-particles[i]->time)/particles[i]->timeToLive;
			particles[i]->color=startColor+(endColor-startColor)*lifePercentage;
			particles[i]->size=startSize+(endSize-startSize)*lifePercentage;
			//particles[i].mat=translate(particles[i].mat,vec3(dir.x,dir.y,dir.z));
			particles[i]->trans=particles[i]->trans+dir;

			shader->setVec3(particles[i]->trans,"trans["+to_string(i)+"]");
			shader->setFloat(lifePercentage,"lifePercentage["+to_string(i)+"]");
			//shader->setVec2(particles[i]->size,"size["+to_string(i)+"]");

			Vector3 v0=particles[i]->trans;
			particles[i]->d=cos(camDir.getAngleBetween((v0-camPos).norm()))*camPos.getDistanceFrom(v0);
		}
		heapSort();
		/*
		for(int i=0;i<numParticles;i++){
			for(int j=i;j<numParticles-1;j++){
				Vector3 v1=particles[j]->trans,v2=particles[j+1]->trans;
				float d1=cos(camDir.getAngleBetween((v1-camPos).norm()))*camPos.getDistanceFrom(v1);
				float d2=cos(camDir.getAngleBetween((v2-camPos).norm()))*camPos.getDistanceFrom(v2);
				if(d1<d2)
					swap(particles[j],particles[j+1]);
			}
		}
		*/
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES,6,GL_UNSIGNED_INT,0,numParticles);	
		for(int i=0;i<numParticles;i++){
		}
			
		//glDisable(GL_CULL_FACE);
		//glEnable(GL_CULL_FACE);
	}

	void ParticleEmitter::setDirection(Vector3 dir){
		this->direction=dir;
		for(int i=0;i<numParticles;i++)
			particles[i]->dir=dir;
	}
}
