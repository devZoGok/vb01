#include<cmath>

namespace vb01{
	class vec3{
		public:
			vec3(float x, float y, float z){
				this->x=x;
				this->y=y;
				this->z=z;
			}
			inline vec3 operator+ (vec3 v){return vec3(this->x+v.x,this->y+v.y,this->z+v.z);}
			inline float dot(vec3 v){return this->x*v.x+this->y*v.y+this->z*v.z;}

			float x,y,z;
		private:
	};
}
