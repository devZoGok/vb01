#include"root.h"
#include"mesh.h"
#include<glad.h>
#include<glfw3.h>
#include<glm.hpp>
#include<ext.hpp>
#include<iostream>
#include<cstdlib>

using namespace std;
using namespace glm;

namespace vb01{
	Mesh::Mesh(){
		
	}

	Mesh::Mesh(Vertex *vertices, unsigned int *indices,int numTris){
		this->vertices=vertices;
		this->indices=indices;
		this->numTris=numTris;
		construct();
	}

	Mesh::~Mesh(){}

	void Mesh::construct(){
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);	
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER, 3*numTris*sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*numTris*sizeof(unsigned int),indices,GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,norm)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,texCoords)));
		glEnableVertexAttribArray(2);

	}

	void Mesh::update(){
		Root *root=Root::getSingleton();
		Camera *cam=root->getCamera();
		float fov=cam->getFov(),width=root->getWidth(),height=root->getHeight();
		float nearPlane=cam->getNearPlane(),farPlane=cam->getFarPlane();
		Vector3 pos=(node?node->getPosition():Vector3::VEC_ZERO);
		Vector3 dir=cam->getDirection(),up=cam->getUp(),camPos=cam->getPosition();

		mat4 model=translate(mat4(1.),vec3(pos.x,pos.y,pos.z));
		model=rotate(model,radians(0.f),vec3(1,1,1));
		mat4 view=lookAt(vec3(camPos.x,camPos.y,camPos.z),vec3(camPos.x+dir.x,camPos.y+dir.y,camPos.z+dir.z),vec3(up.x,up.y,up.z));
		mat4 proj=perspective(radians(fov),width/height,nearPlane,farPlane);
		//proj=ortho(0.f,800.f,0.f,-600.f,nearPlane,farPlane);

		material->update();
		material->getShader()->setVec3(camPos,"camPos");
		material->getShader()->setMat4(model,"model");
		material->getShader()->setMat4(view,"view");
		material->getShader()->setMat4(proj,"proj");

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,3*numTris,GL_UNSIGNED_INT,0);	
	}
}
