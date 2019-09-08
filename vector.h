#ifndef VECTOR_H
#define VECTOR_H

#include<cmath>

namespace vb01{
	struct Vector3{
		Vector3(){};
		Vector3(float x,float y,float z){
			this->x=x;
			this->y=y;
			this->z=z;
		}
		Vector3 operator-(const Vector3 &v){return *this;}
		template<typename T>Vector3 operator+(T s){return Vector3(x+s,y+s,z+s);}
		template<typename T>Vector3 operator-(T s){return Vector3(x-s,y-s,z-s);}
		template<typename T>Vector3 operator*(T s){return Vector3(x*s,y*s,z*s);}
		template<typename T>Vector3 operator/(T s){return Vector3(x/s,y/s,z/s);}
		float getLengthSq(){return x*x+y*y+z*z;}
		float getLength(){return std::sqrt(getLengthSq());}
		float dot(Vector3 v){return x*v.x+y*v.y+z*v.z;}
		float getAngleBetween(Vector3 v){return std::acos(dot(v));}
		Vector3 norm(){return *this/getLength();}
		Vector3 cross(Vector3 v){return Vector3(y*v.z-v.y*z,x*v.z-v.x*z,x*v.y-v.x*y);}

		float x,y,z;
		static const Vector3 VEC_IJK, VEC_I,VEC_J,VEC_K,VEC_ZERO;
	};

	struct Vector2{
		Vector2(){};
		Vector2(float x,float y){
			this->x=x;
			this->y=y;
		}
		template<typename T>Vector2 operator+(T s){return Vector2(x+s,y+s);}
		template<typename T>Vector2 operator-(T s){return Vector2(x-s,y-s);}
		template<typename T>Vector2 operator*(T s){return Vector2(x*s,y*s);}
		template<typename T>Vector2 operator/(T s){return Vector2(x/s,y/s);}
		float getLengthSq(){return x*x+y*y;}
		float getLength(){return std::sqrt(getLengthSq());}
		float dot(Vector2 v){return x*v.x+y*v.y;}
		float getAngleBetween(Vector2 v){return std::acos(dot(v));}
		Vector2 norm(){return *this/getLength();}

		float x,y;
		static const Vector2 VEC_IJ, VEC_I,VEC_J,VEC_ZERO;
	};
}

#endif

