#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "animatable.h"
#include "attachable.h"

#include <vector>
#include <glm.hpp>

namespace vb01{
	class Node;
	class Texture;
	class Shader;
	class Material;

	class Light : public Animatable, public Attachable{
		public:
			enum Type{POINT, DIRECTIONAL, SPOT, AMBIENT};
			enum Attenuation{QUADRATIC, LINEAR, NONE};

			Light(Type, std::string = "");
			~Light();
			void update();
			inline void setColor(Vector3 color){this->color = color;}
			inline void setAttenuationValues(float a, float b, float c){
				attenuationValues.x = a;
				attenuationValues.y = b;
				attenuationValues.z = c;
			}
			inline void setInnerAngle(float innerAngle){this->innerAngle = innerAngle;}
			inline void setOuterAngle(float outerAngle){this->outerAngle = outerAngle;}
			inline void setShadowNearPlane(float nearPlane){this->nearPlane = nearPlane;}
			inline void setShadowFarPlane(float farPlane){this->farPlane = farPlane;}
			inline void setAttenuation(Attenuation at){this->attenuation = at;}
			inline void setRadius(float r){this->radius = r;}
			inline void setAdditiveLighting(bool al){this->additiveLighting = al;}
			inline void setUseAngle(bool ua){this->useAngle = ua;}
			inline Attenuation getAttenuation(){return attenuation;}
			inline Vector3 getAttenuationValues(){return attenuationValues;}
			inline bool isAdditiveLighting(){return additiveLighting;}
			inline float getRadius(){return radius;}
			inline float getInnerAngle(){return innerAngle;}
			inline float getOuterAngle(){return outerAngle;}
			inline float getShadowNearPlane(){return nearPlane;}
			inline float getShadowFarPlane(){return farPlane;}
		private:
			void animate(float, KeyframeChannel);
			void initDepthMap();
			void renderShadow(std::vector<Node*>, glm::mat4&, glm::mat4&);
			void updateShader(std::vector<Material*>, int, glm::mat4&, glm::mat4&);

			Type type;
			bool additiveLighting = true, useAngle = true;
			Attenuation attenuation = Attenuation::QUADRATIC;
			Shader *depthMapShader;
			Texture *depthMap = nullptr;
			Vector3 color = Vector3::VEC_IJK, attenuationValues = Vector3(1.8, .7, 1);
			float innerAngle = .707, outerAngle = .714, nearPlane = .1, farPlane = 100, radius = 0;
			unsigned int depthmapFBO, depthMapSize = 1024;
	};
}

#endif
