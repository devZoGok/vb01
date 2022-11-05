#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "node.h"

class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;

namespace vb01{
	class Mesh;
	class Material;
	class Texture;

	class Model : public Node{
		public:
			Model(){}
			Model(std::string);
			~Model();
			inline Material* getMaterial(){return material;}
			void update();
			void setMaterial(Material*);
			void setCastShadow(bool);
			void setReflect(bool);
			void setReflective(bool);
			void setWireframe(bool);
		private:
			Material* material;
			bool castShadow = false, reflect = false, reflective = false, wireframe = false;
	};
}

#endif
