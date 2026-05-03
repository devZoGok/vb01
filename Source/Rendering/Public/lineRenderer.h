#ifndef LINE_H
#define LINE_H

#include "vector.h"
#include "mesh.h"

#include <vector>

namespace vb01{
		class Shader;

		class LineRenderer{
				public:
						struct Line{int id; Vector3 start; Vector3 end; Vector3 color; bool visible = true;};
						enum VectorFieldType{START, END, COLOR};

						static LineRenderer* getSingleton();
						void drawLines();
						void addLine(Vector3 start, Vector3 end, Vector3 color);
						void changeLineField(int, Vector3, VectorFieldType);
						void toggleVisibility(int i, bool v){getLine(i).visible = v;}
						void removeLine(int);
						Line& getLine(int);
						inline std::vector<Line>& getLines(){return lines;}
				private:
						LineRenderer();

						int nextId = 0;
						Shader *shader = nullptr;
						std::vector<LineRenderer::Line> lines;
		};
}

#endif
