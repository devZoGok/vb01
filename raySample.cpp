#include "root.h"
#include "model.h"
#include "material.h"
#include "assetManager.h"
#include "lineRenderer.h"
#include "ray.h"

using namespace std;
using namespace vb01;

int main(){
		const string PATH = "../", TEX_PATH = PATH + "samples/textures/", MODEL_PATH = PATH + "samples/models/";

		const int numTextures = 6;
		string skyboxTextures[numTextures] = {
			TEX_PATH + "top.jpg",
			TEX_PATH + "bottom.jpg",
			TEX_PATH + "left.jpg",
			TEX_PATH + "right.jpg",
			TEX_PATH + "front.jpg",
			TEX_PATH + "back.jpg"
		};

		for(int i = 0; i < numTextures; i++)
			AssetManager::getSingleton()->load(skyboxTextures[i]);
		
		/*
		 * Gets Root object to start the sample, 
		 * also passes the base path for asset retrieval. 
		*/
		Root *root = Root::getSingleton();
		root->start(800, 600, PATH, "Ray sample");
		root->createSkybox(skyboxTextures);
		
		Camera *cam = root->getCamera();
		cam->setPosition(Vector3(1, 1, 1) * 10);
		cam->lookAt(Vector3(-1, -1, -1).norm(), Vector3(-1, 1, -1).norm());
		
		Node *rootNode = root->getRootNode();

		/*
		 * Creates an icosphere as a target for the ray
		*/
		string modelPath = MODEL_PATH + "sphere.xml";
		AssetManager::getSingleton()->load(modelPath);
		Model *sphModel = new Model(modelPath);
		Material *mat = new Material(PATH + "texture");
		mat->addBoolUniform("lightingEnabled", false);
		mat->addBoolUniform("texturingEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4::VEC_L);
		sphModel->setMaterial(mat);
		rootNode->attachChild(sphModel);
		sphModel->setPosition(Vector3(-4, 0, 0));

		/*
		 * Creates a visual line to represent a castable ray
		*/
		LineRenderer *lineRenderer = LineRenderer::getSingleton();
		float endYPos = -4;
		Vector3 rayStart = Vector3(4, 0, 0);
		lineRenderer->addLine(rayStart, Vector3(-4, endYPos, 0), Vector3::VEC_IJK);
		float shiftSpeed = -.1;

		while(true){
				//Animates the line
				if(endYPos < -4)
						shiftSpeed = .1;
				else if(endYPos > 4)
						shiftSpeed = -.1;

				endYPos += shiftSpeed;
				Vector3 rayEnd = Vector3(-4, endYPos, 0);
				lineRenderer->changeLineField(0, rayEnd, LineRenderer::END);

				/*
				 * Casts an infinite ray that checks for collision against meshes under 
				 * the icoshpere model node and paints it red upon collision
				*/

				mat->setVec4Uniform("diffuseColor", Vector4::VEC_L);
				vector<RayCaster::CollisionResult> results = RayCaster::cast(rayStart, rayEnd - rayStart, sphModel);

				if(!results.empty())
					mat->setVec4Uniform("diffuseColor", Vector4(1, 0, 0, 1));

				root->update();
		}

		return 0;
}
