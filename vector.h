#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

namespace vb01{
	struct Vector4{
		Vector4(){};
		Vector4(float x, float y, float z, float w){
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		Vector4 friend operator-(Vector4 v){return Vector4(-v.x, -v.y, -v.z, -v.w);}
		Vector4 friend operator-(Vector4 v1, Vector4 v2){return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);}
		Vector4 friend operator+(Vector4 v1, Vector4 v2){return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);}
		template<typename T> friend Vector4 operator+(Vector4 v, T s){return Vector4(v.x + s, v.y + s, v.z + s, v.w + s);}
		template<typename T> friend Vector4 operator-(Vector4 v, T s){return Vector4(v.x - s, v.y - s, v.z - s, v.w - s);}
		template<typename T> friend Vector4 operator*(Vector4 v, T s){return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);}
		template<typename T> friend Vector4 operator/(Vector4 v, T s){return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);}
		template<typename T> friend Vector4 operator+(T s, Vector4 v){return Vector4(v.x + s, v.y + s, v.z + s, v.w + s);}
		template<typename T> friend Vector4 operator-(T s, Vector4 v){return Vector4(v.x - s, v.y - s, v.z - s, v.w - s);}
		template<typename T> friend Vector4 operator*(T s, Vector4 v){return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);}
		template<typename T> friend Vector4 operator/(T s, Vector4 v){return Vector4(v.x / s, v.y / s, v.z / s, v.w / s);}
		void operator+=(Vector4 v){x += v.x, y +=v.y, z += v.z, w += v.w;}
		void operator-=(Vector4 v){x -= v.x, y -=v.y, z -= v.z, w -= v.w;}
		template<typename T> void operator+=(T s){x += s, y +=s, z += s, w += s;}
		template<typename T> void operator-=(T s){x -= s, y -=s, z -= s, w -= s;}
		template<typename T> void operator*=(T s){x *= s, y *=s, z *= s, w *= s;}
		template<typename T> void operator/=(T s){x /= s, y /=s, z /= s, w /= s;}

		float x, y, z, w;
		static const Vector4 VEC_IJKL, VEC_I, VEC_J, VEC_K, VEC_L, VEC_ZERO;
	};

	struct Vector3{
		Vector3(){};
		Vector3(float x, float y, float z){
			this->x = x;
			this->y = y;
			this->z = z;
		}
		bool operator!=(const Vector3 &v){return x != v.x || y != v.y || z != v.z;}
		bool operator==(const Vector3 &v){return x == v.x && y == v.y && z == v.z;}
		Vector3 friend operator-(Vector3 v){return Vector3(-v.x, -v.y, -v.z);}
		Vector3 friend operator-(Vector3 v1, Vector3 v2){return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);}
		Vector3 friend operator+(Vector3 v1, Vector3 v2){return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);}
		template<typename T> friend Vector3 operator+(Vector3 v, T s){return Vector3(v.x + s, v.y + s, v.z + s);}
		template<typename T> friend Vector3 operator-(Vector3 v, T s){return Vector3(v.x - s, v.y - s, v.z - s);}
		template<typename T> friend Vector3 operator*(Vector3 v, T s){return Vector3(v.x * s, v.y * s, v.z * s);}
		template<typename T> friend Vector3 operator/(Vector3 v, T s){return Vector3(v.x / s, v.y / s, v.z / s);}
		template<typename T> friend Vector3 operator+(T s, Vector3 v){return Vector3(v.x + s, v.y + s, v.z + s);}
		template<typename T> friend Vector3 operator-(T s, Vector3 v){return Vector3(v.x - s, v.y - s, v.z - s);}
		template<typename T> friend Vector3 operator*(T s, Vector3 v){return Vector3(v.x * s, v.y * s, v.z * s);}
		template<typename T> friend Vector3 operator/(T s, Vector3 v){return Vector3(v.x / s, v.y / s, v.z / s);}
		void operator+=(Vector3 v){x += v.x, y += v.y, z += v.z;}
		void operator-=(Vector3 v){x -= v.x, y -= v.y, z -= v.z;}
		template<typename T> void operator+=(T s){x += s, y += s, z += s;}
		template<typename T> void operator-=(T s){x -= s, y -= s, z -= s;}
		template<typename T> void operator*=(T s){x *= s, y *= s, z *= s;}
		template<typename T> void operator/=(T s){x /= s, y /= s, z /= s;}
		float getLengthSq(){return x * x + y * y + z * z;}
		float getLength(){return std::sqrt(getLengthSq());}
		float dot(Vector3 v){return x * v.x + y * v.y + z * v.z;}
		float getAngleBetween(Vector3 v){
			float dotProd = dot(v);
			if(dotProd > 1.0)
				dotProd = 1.0;
			else if(dotProd < -1)
				dotProd = -1;
			return std::acos(dotProd);
		}
		float getDistanceFrom(Vector3 v){return (v - *this).getLength();}
		Vector3 norm(){return (*this != Vector3::VEC_ZERO ? *this / getLength() : Vector3::VEC_ZERO);}
		Vector3 cross(Vector3 v){return Vector3(y * v.z - v.y * z, z * v.x - x * v.z, x * v.y - v.x * y);}

		float x, y, z;
		static const Vector3 VEC_IJK, VEC_I, VEC_J, VEC_K, VEC_ZERO;
	};

	struct Vector2{
		Vector2(){};
		Vector2(float x, float y){
			this->x = x;
			this->y = y;
		}
		bool operator!=(const Vector2 &v){return x != v.x || y != v.y;}
		bool operator==(const Vector2 &v){return x == v.x && y == v.y;}
		Vector2 friend operator-(Vector2 v){return Vector2(-v.x, -v.y);}
		Vector2 friend operator+(Vector2 v1, Vector2 v2){return Vector2(v1.x + v2.x, v1.y + v2.y);}
		Vector2 friend operator-(Vector2 v1, Vector2 v2){return Vector2(v1.x - v2.x, v1.y - v2.y);}
		template<typename T> friend Vector2 operator+(Vector2 v, T s){return Vector2(v.x + s, v.y + s);}
		template<typename T> friend Vector2 operator-(Vector2 v, T s){return Vector2(v.x - s, v.y - s);}
		template<typename T> friend Vector2 operator*(Vector2 v, T s){return Vector2(v.x * s, v.y * s);}
		template<typename T> friend Vector2 operator/(Vector2 v, T s){return Vector2(v.x / s, v.y / s);}
		template<typename T> friend Vector2 operator+(T s, Vector2 v){return Vector2(v.x + s, v.y + s);}
		template<typename T> friend Vector2 operator-(T s, Vector2 v){return Vector2(v.x - s, v.y - s);}
		template<typename T> friend Vector2 operator*(T s, Vector2 v){return Vector2(v.x * s, v.y * s);}
		template<typename T> friend Vector2 operator/(T s, Vector2 v){return Vector2(v.x / s, v.y / s);}
		void operator+=(Vector2 v){x += v.x, y += v.y;}
		void operator-=(Vector2 v){x -= v.x, y -= v.y;}
		template<typename T> void operator+=(T s){x += s, y += s;}
		template<typename T> void operator-=(T s){x -= s, y -= s;}
		template<typename T> void operator*=(T s){x *= s, y *= s;}
		template<typename T> void operator/=(T s){x /= s, y /= s;}
		float getLengthSq(){return x * x + y * y;}
		float getLength(){return std::sqrt(getLengthSq());}
		float dot(Vector2 v){return x * v.x + y * v.y;}
		float getAngleBetween(Vector2 v){return std::acos(dot(v));}
		float getDistanceFrom(Vector2 v){return (v - *this).getLength();}
		Vector2 norm(){return *this / getLength();}

		float x, y;
		static const Vector2 VEC_IJ, VEC_I, VEC_J, VEC_ZERO;
	};
}

#endif

