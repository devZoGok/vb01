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
	ParticleEmitter::ParticleEmitter(int numParticles){
		this->numParticles=numParticles;

		particles=new Particle[numParticles];

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

		for(int i=0;i<numParticles;i++){
			mat4 mat=translate(mat4(1.f),vec3(0,0,0));
			Particle p;
			p.mat=mat;
			p.dir=direction;
			particles[i]=p;
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
	}

	ParticleEmitter::~ParticleEmitter(){
		glDeleteVertexArrays(1,&VAO);	
		glDeleteBuffers(1,&VBO);	

		delete material;

		delete[] particles;
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

		Vector3 nodePos=node->getWorldTransform().position;
		Vector3 nodeDir=node->getLocalAxis(2);
		Vector3 nodeUp=node->getLocalAxis(1);
		Vector3 nodeLeft=node->getLocalAxis(0);
		for(int i=0;i<numParticles;i++){
			Vector3 dir=particles[i].dir;
			if(getTime()-particles[i].time>=particles[i].timeToLive){
				particles[i].time=getTime();
				float factor=(float)(rand()%100)/100;
				float s1=float(rand()%(int)spread)/180*PI,s2=float(rand()%360)/180*PI;
				Vector3 ra1=(Vector3(0,1,0).cross(dir)).norm(),ra2=dir.norm();
				dir=Quaternion(s1,ra1)*dir;
				dir=Quaternion(s2,ra2)*dir;
				particles[i].timeToLive=s64(1000*((highLife-lowLife)*factor+lowLife));
				particles[i].mat=translate(mat4(1.f),vec3(nodePos.x,nodePos.y,nodePos.z));
			}
			//dir=nodeLeft*dir.x+nodeUp*dir.y+nodeDir*dir.z;
			particles[i].dir=dir.norm()*direction.getLengthSq()+gravity;
			float lifePercentage=(float)(getTime()-particles[i].time)/particles[i].timeToLive;
			particles[i].color=startColor+(endColor-startColor)*lifePercentage;
			particles[i].size=startSize+(endSize-startSize)*lifePercentage;
			particles[i].mat=translate(particles[i].mat,vec3(dir.x,dir.y,dir.z));

		}
		for(int i=0;i<numParticles;i++){
			for(int j=0;j<numParticles-1;j++){
				mat4 m1=particles[j].mat,m2=particles[j+1].mat;
				Vector3 v1=Vector3(m1[3][0],m1[3][1],m1[3][2]);
				Vector3 v2=Vector3(m2[3][0],m2[3][1],m2[3][2]);
				float d1=cos(camDir.getAngleBetween((v1-camPos).norm()))*camPos.getDistanceFrom(v1);
				float d2=cos(camDir.getAngleBetween((v2-camPos).norm()))*camPos.getDistanceFrom(v2);
				if(d1<d2){
					swap(particles[j],particles[j+1]);
				}
			}
		}
		for(int i=0;i<numParticles;i++){
			shader->setMat4(particles[i].mat,"model["+to_string(i)+"]");
			shader->setVec4(particles[i].color,"color["+to_string(i)+"]");
			shader->setVec2(particles[i].size,"size["+to_string(i)+"]");
		}
			
		//glDisable(GL_CULL_FACE);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES,6,GL_UNSIGNED_INT,0,numParticles);	
		//glEnable(GL_CULL_FACE);
	}

	void ParticleEmitter::setDirection(Vector3 dir){
		this->direction=dir;
		for(int i=0;i<numParticles;i++)
			particles[i].dir=dir;
	}
}
