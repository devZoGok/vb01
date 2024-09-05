#include "xmlModelReader.h"
#include "mesh.h"
#include "skeleton.h"
#include "bone.h"
#include "animation.h"
#include "vector.h"
#include "animationController.h"
#include "animatable.h"
#include "modelAsset.h"

#include <vector>

#include <tinyxml2.h>

using namespace std;
using namespace tinyxml2;

namespace vb01{
		XmlModelReader *xmlModelReader = nullptr;

		XmlModelReader* XmlModelReader::getSingleton(){
				if(!xmlModelReader)
						xmlModelReader = new XmlModelReader();

				return xmlModelReader;
		}

		Asset* XmlModelReader::readAsset(string path){
				XMLDocument *doc = new XMLDocument();
				doc->LoadFile(path.c_str());

				XMLElement *root = doc->FirstChildElement();
				ModelAsset *asset = new ModelAsset();
				asset->path = path;
				asset->rootNode = new Node();
				assetRootNode = asset->rootNode;

				for(XMLElement *ch = root->FirstChildElement(); ch; ch = ch->NextSiblingElement())
					assetRootNode->attachChild(processNode(assetRootNode, ch));

				vector<Node*> descendants;
				assetRootNode->getDescendants(descendants);
				setupDrivers(descendants);

				delete doc;

				return asset;
		}

		void XmlModelReader::setupDrivers(vector<Node*> descendants){
				for(pair<string, Driver*> pair : driversByNodeNames){
					for(Node *desc : descendants){
						int dotId = pair.first.find_first_of(".");
						string driverNodeName = (dotId == string::npos ? pair.first : pair.first.substr(dotId + 1));

						if(driverNodeName == desc->getName())
								desc->addDriver(pair.second);
					}
				}
		}

		//TODO remove the 4 literal for bone weights and indices
		Mesh* XmlModelReader::processMesh(XMLElement *meshEl){
				int numVertPos = atoi(meshEl->Attribute("num_vertex_pos"));
				int numVertexGroups = atoi(meshEl->Attribute("num_vertex_groups"));

				const char *tagName = "vertdata";

				Vector3 *vertPos = new Vector3[numVertPos];
				float **weights = new float*[numVertPos];
				int **boneIndices = new int*[numVertPos], i = 0;

				for(XMLElement *vertEl = meshEl->FirstChildElement(tagName); vertEl && strcmp(vertEl->Name(), tagName) == 0, i < numVertPos; vertEl = vertEl->NextSiblingElement(), i++){
						float posX = atof(vertEl->Attribute("px"));
						float posY = atof(vertEl->Attribute("py"));
						float posZ = atof(vertEl->Attribute("pz"));
						vertPos[i] = Vector3(posX, posY, posZ);

						weights[i] = new float[4];
						boneIndices[i] = new int[4];

						for(int j = 0; j < 4; j++){
							weights[i][j] = 0;
							boneIndices[i][j] = -1;
						}

						int boneIndex = 0, j = 0;

						for(XMLElement *weightEl = vertEl->FirstChildElement("weight"); weightEl, j < numVertexGroups; weightEl = weightEl->NextSiblingElement(), j++){
							float w = atof(weightEl->Attribute("value"));

							if(w > 0){
								weights[i][boneIndex] = w;
								boneIndices[i][boneIndex] = j;
								boneIndex++;
							}
						}
				}

				string *vertexGroups = new string[numVertexGroups];
				tagName = "vertexgroup";
				i = 0;

				if(numVertexGroups > 0)
					for(XMLElement *vertGroupEl = meshEl->FirstChildElement(tagName); vertGroupEl && strcmp(vertGroupEl->Name(), tagName) == 0 && i < numVertexGroups; vertGroupEl = vertGroupEl->NextSiblingElement(), i++)
						vertexGroups[i] = vertGroupEl->Attribute("name");

				i = 0;
				int numVertices = 3 * atoi(meshEl->Attribute("num_faces"));
				MeshData::Vertex *vertices = new MeshData::Vertex[numVertices];
				tagName = "vert";
				u32 *indices = new u32[numVertices];

				Vector3 *normals = new Vector3[numVertices];
				vector<int> vertIds;

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

						normals[i] = tan.cross(biTan);

						MeshData::Vertex vertex;
						vertex.pos = &vertPos[id];
						vertex.norm = &normals[id];
						vertex.uv = uv;
						vertex.tan = tan;
						vertex.biTan = biTan;

						vertex.weights = weights[id];
						vertex.boneIndices = boneIndices[id];

						indices[i] = i;
						vertices[i] = vertex;
				}

				tagName = "shapekey";
				i = 0;
				int numShapeKeys = atoi(meshEl->Attribute("num_shape_keys"));
				MeshData::ShapeKey *shapeKeys = nullptr;
				Vector3 **shapeKeyOffsets = nullptr;

				if(numShapeKeys > 0){
					shapeKeys = new MeshData::ShapeKey[numShapeKeys];
					shapeKeyOffsets = new Vector3*[numVertPos];

					for(int i = 0; i < numVertPos; i++)
						shapeKeyOffsets[i] = new Vector3[100];

					for(XMLElement *shapeKeyEl = meshEl->FirstChildElement(tagName); shapeKeyEl && strcmp(shapeKeyEl->Name(), tagName) == 0 && i < numShapeKeys; shapeKeyEl = shapeKeyEl->NextSiblingElement(), i++){
							string name = shapeKeyEl->Attribute("name");
							float minValue = atof(shapeKeyEl->Attribute("min"));
							float maxValue = atof(shapeKeyEl->Attribute("max"));
							shapeKeys[i] = MeshData::ShapeKey(name, minValue, minValue, maxValue);

							XMLElement *driverEl = shapeKeyEl->FirstChildElement("driver");
							KeyframeChannel channel = processKeyframeChannells(driverEl)[0];
							const char *nodeName = driverEl->Attribute("obj");
							const char *boneName = driverEl->Attribute("bone");
							Driver::VariableType type = Driver::getDriverVariableType(driverEl->Attribute("type"));
							driversByNodeNames.push_back(make_pair(string(nodeName) + (boneName ? "." + string(boneName) : ""), new Driver(&shapeKeys[i], channel, type)));

							int j = 0;

							for(XMLElement *vertEl = shapeKeyEl->FirstChildElement("vert"); vertEl; vertEl = vertEl->NextSiblingElement(), j++){
									float posX = atof(vertEl->Attribute("px"));
									float posY = atof(vertEl->Attribute("py"));
									float posZ = atof(vertEl->Attribute("pz"));
									shapeKeyOffsets[j][i] = Vector3(posX, posY, posZ) - vertPos[j];
							}
					}

					for(int i = 0; i < numVertices; i++)
						vertices[i].shapeKeyOffsets = shapeKeyOffsets[vertIds[i]];
				}

			const char *fullSkeletonName = meshEl->Attribute("skeleton");
			string name = string(meshEl->Parent()->ToElement()->Attribute("name")) + string(meshEl->Attribute("name"));
			Mesh *mesh = new Mesh(MeshData(vertPos, weights, boneIndices, shapeKeyOffsets, numVertPos, normals, vertices, indices, numVertices / 3, name, vertexGroups, numVertexGroups, (fullSkeletonName ? fullSkeletonName : ""), shapeKeys, numShapeKeys));

			return mesh;
		}

		vector<KeyframeChannel> XmlModelReader::processKeyframeChannells(XMLElement *containerTag){
				vector<KeyframeChannel> keyframeChannels;

				for(XMLElement *channelEl = containerTag->FirstChildElement("channel"); channelEl; channelEl = channelEl->NextSiblingElement()){
						vector<Keyframe> keyframes;

						for(XMLElement *keyframeEl = channelEl->FirstChildElement("keyframe"); keyframeEl; keyframeEl = keyframeEl->NextSiblingElement()){
								string interpString = keyframeEl->Attribute("interpolation");
								Keyframe::Interpolation interpolation = KeyframeChannel::getKeyframeInterpolationType(interpString);
								float frame = atof(keyframeEl->Attribute("frame"));
								float value = atof(keyframeEl->Attribute("value"));
								float leftFrame = atof(keyframeEl->Attribute("lefthandleframe"));
								float leftValue = atof(keyframeEl->Attribute("lefthandlevalue"));
								float rightFrame = atof(keyframeEl->Attribute("righthandleframe"));
								float rightValue = atof(keyframeEl->Attribute("righthandlevalue"));
								keyframes.push_back(KeyframeChannel::createKeyframe(interpolation, value, frame, leftValue, leftFrame, rightValue, rightFrame));
						}

						string typeStr = channelEl->Attribute("type");
						KeyframeChannel::Type type = KeyframeChannel::getKeyframeChannelType(typeStr);
						string channelName = channelEl->Attribute("name");
						keyframeChannels.push_back(KeyframeChannel::createKeyframeChannel(type, channelName, keyframes));
				}

				return keyframeChannels;
		}

		void XmlModelReader::processAnimations(XMLElement *animContainer, vector<Animatable*> animatables){
				AnimationController *animController = AnimationController::getSingleton();
				const char *animTag = "animation";

				for(XMLElement *animEl = animContainer->FirstChildElement(animTag); animEl && strcmp(animEl->Name(), animTag) == 0; animEl = animEl->NextSiblingElement()){
					string name = animEl->Attribute("name");
					Animation *animation = new Animation(name);
					animation->addKeyframeChannels(processKeyframeChannells(animEl));
					animController->addAnimation(animation);
				}
		}

		Skeleton* XmlModelReader::processSkeleton(Node *root, XMLElement *skeletonEl){
				XMLElement *rootBoneEl = skeletonEl->FirstChildElement("bone");
				Bone *rootBone = (Bone*)processNode(root, rootBoneEl, true);
				root->attachChild(rootBone);
				Skeleton *skeleton = new Skeleton(skeletonEl->Attribute("name"));

				vector<Node*> bones = vector<Node*>{rootBone};
				rootBone->getDescendants(bones);

				for(Node *bone : bones)
						skeleton->addBone((Bone*)bone, nullptr);

				processAnimations(skeletonEl, vector<Animatable*>(bones.begin(), bones.end()));

				return skeleton;
		}

		Node* XmlModelReader::processNode(Node *parent, XMLElement *xmlEl, bool bone){
				Node *node = nullptr;
				const char *name = xmlEl->Attribute("name");

				if(bone){
						float components[]{
							atof(xmlEl->Attribute("hx")),
							atof(xmlEl->Attribute("hy")),
							atof(xmlEl->Attribute("hz")),
							atof(xmlEl->Attribute("xaxisx")),
							atof(xmlEl->Attribute("xaxisy")),
							atof(xmlEl->Attribute("xaxisz")),
							atof(xmlEl->Attribute("yaxisx")),
							atof(xmlEl->Attribute("yaxisy")),
							atof(xmlEl->Attribute("yaxisz"))
						};

						float eps = pow(10, -2);
						
						for(int i = 0; i < 9; i++)
								if(fabs(components[i]) < eps)
										components[i] = 0;

						Vector3 head = Vector3(components[0], components[1], components[2]);
						Vector3 xAxis = Vector3(components[3], components[4], components[5]);
						Vector3 yAxis = Vector3(components[6], components[7], components[8]);
						float length = atof(xmlEl->Attribute("length"));
						Vector3 zAxis = xAxis.cross(yAxis).norm();
						Vector3 pos = head;
						
						if(strcmp(xmlEl->Parent()->ToElement()->Name(), "skeleton") == 0){
							parent = assetRootNode;
							swap(pos.y, pos.z);
							pos.z = -pos.z;
							swap(yAxis.y, yAxis.z);
							yAxis.z = -yAxis.z;
							swap(zAxis.y, zAxis.z);
							zAxis.z = -zAxis.z;
						}
						else
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
				else{
					Vector3 pos = Vector3(atof(xmlEl->Attribute("px")), atof(xmlEl->Attribute("py")), atof(xmlEl->Attribute("pz")));
					Quaternion rot = Quaternion(atof(xmlEl->Attribute("rw")), atof(xmlEl->Attribute("rx")), atof(xmlEl->Attribute("ry")), atof(xmlEl->Attribute("rz")));
					Vector3 scale = Vector3(atof(xmlEl->Attribute("sx")), atof(xmlEl->Attribute("sy")), atof(xmlEl->Attribute("sz")));
					node = new Node(pos, rot, scale, name);
				}

				XMLElement *skeletonTag = xmlEl->FirstChildElement("skeleton");

				if(skeletonTag)
						node->addSkeleton(processSkeleton(node, skeletonTag));

				XMLElement *meshTag = xmlEl->FirstChildElement("mesh");

				if(meshTag)
					node->attachMesh(processMesh(meshTag));

				XMLElement *lightTag = xmlEl->FirstChildElement("light");

				processAnimations(xmlEl, vector<Animatable*>{node});

				const char *tagName = (bone ? "bone" : "node");

				for(XMLElement *el = xmlEl->FirstChildElement(tagName); el; el = el->NextSiblingElement())
					node->attachChild(processNode(node, el, bone));

				return node;
		}
}
