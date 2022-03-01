#include "xmlModelReader.h"
#include "mesh.h"
#include "skeleton.h"
#include "bone.h"
#include "vector.h"

#include <vector>

#include <tinyxml2.h>

using namespace std;
using namespace tinyxml2;

namespace vb01{
		XmlModelReader::XmlModelReader(Model *model, string path) : ModelReader(model, path){
				XMLDocument *doc = new XMLDocument();
				doc->LoadFile(path.c_str());

				XMLNode *root = doc->FirstChild();
				model->attachChild(processNode(model, (XMLElement*)root));
		}

		Mesh* XmlModelReader::processMesh(XMLElement *meshEl){
				vector<Vector3> vertPos, vertNorm;
				vector<float> weights;
				const char *tagName = "vertdata";

				for(XMLElement *vertEl = meshEl->FirstChildElement(tagName); vertEl && strcmp(vertEl->Name(), tagName) == 0; vertEl = vertEl->NextSiblingElement()){
						float posX = atof(vertEl->Attribute("px"));
						float posY = atof(vertEl->Attribute("py"));
						float posZ = atof(vertEl->Attribute("pz"));
						vertPos.push_back(Vector3(posX, posY, posZ));

						float normX = atof(vertEl->Attribute("nx"));
						float normY = atof(vertEl->Attribute("ny"));
						float normZ = atof(vertEl->Attribute("nz"));
						vertNorm.push_back(Vector3(normX, normY, normZ));

						for(XMLElement *weightEl = vertEl->FirstChildElement("weight"); weightEl; weightEl = weightEl->NextSiblingElement())
								weights.push_back(atof(weightEl->Attribute("value")));
				}

				int numVertexGroups = atoi(meshEl->Attribute("num_vertex_groups"));
				string *vertexGroups = new string[numVertexGroups];
				tagName = "vertexgroup";
				int i = 0;

				for(XMLElement *vertGroupEl = meshEl->FirstChildElement(tagName); vertGroupEl && strcmp(vertGroupEl->Name(), tagName) == 0; vertGroupEl = vertGroupEl->NextSiblingElement(), i++)
						vertexGroups[i] = (vertGroupEl->Attribute("name"));

				i = 0;
				int numVertices = 3 * atoi(meshEl->Attribute("num_faces"));
				int numBones;
				Mesh::Vertex *vertices = new Mesh::Vertex[numVertices];
				tagName = "vert";
				vector<u32> vertIds;
				u32 *indices = new u32[numVertices];

				for(XMLElement *vertEl = meshEl->FirstChildElement(tagName); vertEl && strcmp(vertEl->Name(), tagName) == 0; vertEl = vertEl->NextSiblingElement(), i++){
						int id = atoi(vertEl->Attribute("id"));
						vertIds.push_back(id);

						float uvX = atof(vertEl->Attribute("uvx"));
						float uvY = atof(vertEl->Attribute("uvy"));
						Vector2 uv = Vector2(uvX, uvY);

						float tanX = atof(vertEl->Attribute("tx"));
						float tanY = atof(vertEl->Attribute("ty"));
						float tanZ = atof(vertEl->Attribute("tz"));
						Vector3 tan = Vector3(tanX, tanY, tanZ);

						float biTanX = atof(vertEl->Attribute("bx"));
						float biTanY = atof(vertEl->Attribute("by"));
						float biTanZ = atof(vertEl->Attribute("bz"));
						Vector3 biTan = Vector3(biTanX, biTanY, biTanZ);

						Mesh::Vertex vertex;
						vertex.pos = vertPos[id];
						vertex.norm = vertNorm[id];
						vertex.uv = uv;
						vertex.tan = tan;
						vertex.biTan = biTan;

						for(int j = 0, boneIndex = 0; j < numBones; j++){
							if(weights[id * numBones + j] > 0){
								vertex.boneIndices[boneIndex] = j;
								vertex.weights[boneIndex] = weights[id * numBones + j];
								boneIndex++;
							}
						}

						indices[i] = i;
						vertices[i] = vertex;
				}

				vertNorm.clear();
				tagName = "shapekey";
				i = 0;
				int numShapeKeys = atoi(meshEl->Attribute("num_shape_keys"));
				Mesh::ShapeKey *shapeKeys = new Mesh::ShapeKey[numShapeKeys];

				for(XMLElement *shapeKeyEl = meshEl->FirstChildElement(tagName); shapeKeyEl && strcmp(shapeKeyEl->Name(), tagName) == 0; shapeKeyEl = shapeKeyEl->NextSiblingElement(), i++){
						shapeKeys[i].name = shapeKeyEl->Attribute("name");
						shapeKeys[i].minValue = atof(shapeKeyEl->Attribute("min"));
						shapeKeys[i].maxValue = atof(shapeKeyEl->Attribute("max"));

						XMLElement *driverEl = shapeKeyEl->FirstChildElement("driver");

						vector<Vector3> shapeKeyPos;

						for(XMLElement *vertEl = shapeKeyEl->FirstChildElement(tagName); vertEl; vertEl = vertEl->NextSiblingElement()){
								float posX = atof(vertEl->Attribute("px"));
								float posY = atof(vertEl->Attribute("py"));
								float posZ = atof(vertEl->Attribute("pz"));
								shapeKeyPos.push_back(Vector3(posX, posY, posZ));

								for(int j = 0; j < numVertices; j++)
										vertices[j].shapeKeyOffsets[i] = shapeKeyPos[vertIds[j]] - vertPos[vertIds[j]];
						}
				}

			Mesh *mesh = new Mesh(vertices, indices, numVertices / 3, vertexGroups, numBones, shapeKeys, numShapeKeys);
			return mesh;
		}

		Skeleton* XmlModelReader::processSkeleton(Node *root, XMLElement *skeletonEl){
				XMLElement *rootBoneEl = skeletonEl->FirstChildElement("bone");
				Skeleton *skeleton = new Skeleton(nullptr);
				skeleton->addBone((Bone*)processNode(root, rootBoneEl, true), (Bone*)root);

				return skeleton;
		}

		Node* XmlModelReader::processNode(Node *parent, XMLElement *xmlEl, bool bone){
				Node *node = nullptr;

				if(bone){
						float headX = atof(xmlEl->Attribute("hx"));
						float headY = atof(xmlEl->Attribute("hy"));
						float headZ = atof(xmlEl->Attribute("hz"));
						Vector3 head = Vector3(headX, headY, headZ);

						float xAxisX = atof(xmlEl->Attribute("xaxisx"));
						float xAxisY = atof(xmlEl->Attribute("xaxisy"));
						float xAxisZ = atof(xmlEl->Attribute("xaxisz"));
						Vector3 xAxis = Vector3(xAxisX, xAxisY, xAxisZ);

						float yAxisX = atof(xmlEl->Attribute("yaxisx"));
						float yAxisY = atof(xmlEl->Attribute("yaxisy"));
						float yAxisZ = atof(xmlEl->Attribute("yaxisz"));
						Vector3 yAxis = Vector3(yAxisX, yAxisY, yAxisZ);

						string name = xmlEl->Attribute("name");
						float length = atof(xmlEl->Attribute("length"));

						Vector3 zAxis = xAxis.cross(yAxis).norm();
						Vector3 pos = head;
						
						/*
						if(!parent){
							parent = model;
							swap(pos.y, pos.z);
							pos.z = -pos.z;
							swap(yAxis.y, yAxis.z);
							yAxis.z = -yAxis.z;
							swap(zAxis.y, zAxis.z);
							zAxis.z = -zAxis.z;
						}
						else
						*/
							pos = pos + Vector3(0, ((Bone*)parent)->getLength(), 0);
						
						node = new Bone(name, length, pos, Quaternion::QUAT_W, Vector3::VEC_IJK);
						node->lookAt(zAxis, yAxis);
						const char *ikTarget = xmlEl->Attribute("iktarget");

						if(ikTarget != nullptr){
								((Bone*)node)->setIkTarget(ikTarget);
							int ikChainLength = atoi(xmlEl->Attribute("chainlength"));
							((Bone*)node)->setIkChainLength(ikChainLength);
						}
				}

				XMLElement *skeletonTag = xmlEl->FirstChildElement("skeleton");

				if(skeletonTag)
						processSkeleton(node, skeletonTag);

				XMLElement *meshTag = xmlEl->FirstChildElement("mesh");

				if(meshTag)
						processMesh(meshTag);

				XMLElement *lightTag = xmlEl->FirstChildElement("light");

				for(XMLElement *el = xmlEl->FirstChildElement("node"); el; el = el->NextSiblingElement())
					parent->attachChild(processNode(parent, el, bone));

				return node;
		}
}
