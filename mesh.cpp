#include"root.h"
#include"node.h"
#include"mesh.h"
#include"skeleton.h"
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

	Mesh::~Mesh(){
		for(Mesh *m : meshes)
			delete m;

		delete material;

		glDeleteVertexArrays(1,&VAO);
		glDeleteBuffers(1,&EBO);
		glDeleteBuffers(1,&VBO);

		delete[] indices;
		delete[] vertices;
	}

	void Mesh::construct(){
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);	
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER, 3*numTris*sizeof(Vertex),staticVerts?vertices:NULL, staticVerts?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*numTris*sizeof(unsigned int),staticVerts?indices:NULL,staticVerts?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);

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
		Vector3 dir=cam->getDirection(),up=cam->getUp(),camPos=Vector3::VEC_ZERO,pos=Vector3::VEC_ZERO,scale=Vector3::VEC_IJK;
		Quaternion orient=Quaternion::QUAT_W;

		if(node){
			Node::Transform t=node->getWorldTransform();

			pos=t.position;
			orient=t.orientation;
			camPos=cam->getPosition();
		}

		Vector3 rotAxis=orient.norm().getAxis();
		if(rotAxis==Vector3::VEC_ZERO)
			rotAxis=Vector3::VEC_I;
		mat4 model=translate(mat4(1.),vec3(pos.x,pos.y,pos.z));
		model=rotate(model,orient.norm().getAngle(),vec3(rotAxis.x,rotAxis.y,rotAxis.z));
		model=glm::scale(model,vec3(scale.x,scale.y,scale.z));
		mat4 view=lookAt(vec3(camPos.x,camPos.y,camPos.z),vec3(camPos.x+dir.x,camPos.y+dir.y,camPos.z+dir.z),vec3(up.x,up.y,up.z));
		mat4 proj=perspective(radians(fov),width/height,nearPlane,farPlane);

		material->update();
		Shader *shader=material->getShader();
		shader->setBool(castShadow,"castShadow");
		shader->setMat4(view,"view");
		shader->setMat4(proj,"proj");
		shader->setVec3(camPos,"camPos");
		shader->setMat4(model,"model");
		if(skeleton){
		}
		if(material->getType()==Material::MATERIAL_GUI){
			shader->setVec2(Vector2((float)width,(float)height),"screen");
			if(node)
				shader->setVec3(pos,"pos");
		}

		render();
	}

	void Mesh::render(){
		glBindVertexArray(VAO);
		if(!staticVerts){
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 3*numTris*sizeof(Vertex),vertices);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,3*numTris*sizeof(unsigned int),indices);
		}
		glPolygonMode(GL_FRONT_AND_BACK,wireframe?GL_LINE:GL_FILL);
		glDrawElements(GL_TRIANGLES,3*numTris,GL_UNSIGNED_INT,0);	
	}
}
