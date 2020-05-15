#include"root.h"
#include"node.h"
#include"box.h"
#include"texture.h"
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

		if(material)
			delete material;

		glDeleteVertexArrays(1,&VAO);
		glDeleteBuffers(1,&EBO);
		glDeleteBuffers(1,&VBO);

		delete[] indices;
		delete[] vertices;
	}

	void Mesh::construct(){
		u32 RBO;
		string basePath="../../vb01/depthMap.";
		environmentShader=new Shader(basePath+"vert",basePath+"frag",basePath+"geo");
		environmentMap=new Texture(width,false);

		glGenFramebuffers(1,&environmentBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER,environmentBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,*(environmentMap->getTexture()),0);

		/*
		glGenRenderbuffers(1,&RBO);
		glBindRenderbuffer(GL_RENDERBUFFER,RBO);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,width,width);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);
		*/

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
			cout<<"Not complete\n";

		glBindFramebuffer(GL_FRAMEBUFFER,0);

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
		glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,tan)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(offsetof(Vertex,biTan)));
		glEnableVertexAttribArray(4);
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

		if(reflect){
			glViewport(0,0,width,width);
			glBindFramebuffer(GL_FRAMEBUFFER,environmentBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			vec3 dirs[]{
				vec3(1,0,0),
				vec3(-1,0,0),
				vec3(0,1,0),
				vec3(0,-1,0),
				vec3(0,0,1),
				vec3(0,0,-1)
			};
			environmentShader->use();
			Node *rootNode=root->getRootNode();
			vector<Node*> descendants;
			rootNode->getDescendants(rootNode,descendants);
			for(Node *n : descendants){
				for(Mesh *m : n->getMeshes())
					if(m!=this){
						Vector3 position=n->getWorldPosition();
						Quaternion rotation=n->getWorldOrientation();
						Vector3 rotAxis=rotation.norm().getAxis();

						if(rotAxis==Vector3::VEC_ZERO)
							rotAxis=Vector3::VEC_I;
						float far=100;
						vec3 p=vec3(position.x,position.y,position.z);
						mat4 proj=perspective(radians(90.f),1.f,.1f,far);
						mat4 model=translate(mat4(1.),p);
						model=rotate(model,rotation.norm().getAngle(),vec3(rotAxis.x,rotAxis.y,rotAxis.z));
						environmentShader->setMat4(model,"model");
						environmentShader->setBool(true,"point");
						environmentShader->setVec3(pos,"lightPos");
						environmentShader->setFloat(far,"farPlane");
						for(int i=0;i<6;i++)
							environmentShader->setMat4(proj*lookAt(p,p+dirs[i],1<i&&i<4?vec3(0,0,-1):vec3(0,-1,0)),"shadowMat["+to_string(i)+"]");
						m->render();
					}
			}
			glBindFramebuffer(GL_FRAMEBUFFER,*(root->getFBO()));
			glViewport(0,0,root->getWidth(),root->getHeight());
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
		shader->setBool(false,"environment");
		shader->setMat4(view,"view");
		shader->setMat4(proj,"proj");
		shader->setVec3(camPos,"camPos");
		shader->setMat4(model,"model");
		if(reflect){
			shader->setBool(reflect,"environmentMapEnabled");
			root->getSkybox()->getMaterial()->getDiffuseMap(0)->select(4);
			environmentMap->select(4);
		}
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
