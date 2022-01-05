#include "lineRenderer.h"
#include "root.h"

#include "glad.h"

#include <glm.hpp>
#include <ext.hpp>

using namespace std;
using namespace glm;

namespace vb01{
		LineRenderer *lineRenderer = nullptr;

		LineRenderer* LineRenderer::getSingleton(){
				if(!lineRenderer)
						lineRenderer = new LineRenderer();

				return lineRenderer;
		}

		LineRenderer::LineRenderer(){
				shader = new Shader(Root::getSingleton()->getLibPath() + "line3D");
		}

		void LineRenderer::drawLines(){
			Root *root = Root::getSingleton();
			Camera *cam = root->getCamera();

			Vector3 dir = cam->getDirection(), up = cam->getUp();
			Vector3 camPos = cam->getPosition();
			mat4 view = lookAt(vec3(camPos.x, camPos.y, camPos.z), vec3(camPos.x + dir.x, camPos.y + dir.y, camPos.z + dir.z), vec3(up.x, up.y, up.z));

			float fov = cam->getFov(), width = root->getWidth(), height = root->getHeight(), nearPlane = cam->getNearPlane(), farPlane = cam->getFarPlane();
			mat4 proj = perspective(radians(fov), width / height, nearPlane, farPlane);

			for(Line &l : lines){
				if(l.visible){
					shader->use();
					shader->setVec3(l.color, "color");
					shader->setMat4(proj, "proj");
					shader->setMat4(view, "view");

					glBegin(GL_LINES);
					glVertex3f(l.start.x, l.start.y, l.start.z);
					glVertex3f(l.end.x, l.end.y, l.end.z);
					glEnd();
				}
			}
		}

		void LineRenderer::addLine(Vector3 start, Vector3 end, Vector3 color){
				Line line;
				line.id = nextId;
				line.start = start;
				line.end = end;
				line.color = color;
				lines.push_back(line);

				nextId++;
		}

		LineRenderer::Line& LineRenderer::getLine(int i){
				for(Line &l : lines)
						if(l.id == i)
								return l;
		}

		void LineRenderer::changeLineField(int id, Vector3 vec, VectorFieldType field){
				Line &line = LineRenderer::getLine(id);

				switch(field){
						case VectorFieldType::START:
								line.start = vec;
								break;
						case VectorFieldType::END:
								line.end = vec;
								break;
						case VectorFieldType::COLOR:
								line.color= vec;
								break;
				}
		}

		void LineRenderer::removeLine(int id){
				for(int i = 0; i < lines.size(); ++i)
						if(lines[i].id == id){
								lines.erase(lines.begin() + i);
								break;
						}
		}
}
