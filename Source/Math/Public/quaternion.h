#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

#include "vector.h"

namespace vb01{
	class Quaternion{
		public:
			Quaternion(float w, float x, float y, float z){
				this->w = w;
				this->x = x;
				this->y = y;
				this->z = z;
			}
			Quaternion(float angle, Vector3 axis){
				*this = this->fromAngle(angle, axis);
			}
			Quaternion(){}
			inline bool operator==(Quaternion q){return w == q.w && x == q.x && y == q.y && z == q.z;}
			inline bool operator!=(Quaternion q){return w != q.w || x != q.x || y != q.y || z != q.z;}
			inline friend Quaternion operator-(Quaternion q){return Quaternion(q.w, -q.x, -q.y, -q.z);}
			inline friend Quaternion operator+(Quaternion q1, Quaternion q2){return Quaternion(q1.w + q2.w, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);}
			inline friend Quaternion operator-(Quaternion q1, Quaternion q2){return Quaternion(q1.w - q2.w, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);}
			inline Quaternion operator* (Quaternion q){
				float a1 = this->w, a2 = q.w,
					  b1 = this->x, b2 = q.x,
					  c1 = this->y, c2 = q.y,
					  d1 = this->z, d2 = q.z;
				float w = a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2;
				float x = a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2;
				float y = a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2;
				float z = a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2;
				return Quaternion(w, x, y, z);
			}
			inline Vector3 operator* (Vector3 v){
				Quaternion vQuat = (*this) * Quaternion(0, v.x, v.y, v.z) * recip();
				return Vector3(vQuat.x, vQuat.y, vQuat.z);
			}
			inline float getAngle(){return fabs(w) > 1 ? 0 : 2 * acos(w);}
			inline Vector3 getAxis(){return x == 0 && y == 0 && z == 0 ? Vector3(1, 0, 0) : Vector3(x, y, z).norm();}
			template<typename T> friend Quaternion operator+(Quaternion q, T s){return Quaternion(q.w + s, q.x + s, q.y + s, q.z + s);}
			template<typename T> friend Quaternion operator-(Quaternion q, T s){return Quaternion(q.w - s, q.x - s, q.y - s, q.z - s);}
			template<typename T> friend Quaternion operator*(Quaternion q, T s){return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);}
			template<typename T> friend Quaternion operator/(Quaternion q, T s){return Quaternion(q.w / s, q.x / s, q.y / s, q.z / s);}
			template<typename T> friend Quaternion operator+(T s, Quaternion q){return Quaternion(q.w + s, q.x + s, q.y + s, q.z + s);}
			template<typename T> friend Quaternion operator-(T s, Quaternion q){return Quaternion(q.w - s, q.x - s, q.y - s, q.z - s);}
			template<typename T> friend Quaternion operator*(T s, Quaternion q){return Quaternion(q.w * s, q.x * s, q.y * s, q.z * s);}
			template<typename T> friend Quaternion operator/(T s, Quaternion q){return Quaternion(q.w / s, q.x / s, q.y / s, q.z / s);}
			void operator+=(Quaternion q){w += q.w, x += q.x, y += q.y, z += q.z;}
			void operator-=(Quaternion q){w -= q.w, x -= q.x, y -= q.y, z -= q.z;}
			void operator*=(Quaternion q){
				float a1 = this->w, a2 = q.w,
					  b1 = this->x, b2 = q.x,
					  c1 = this->y, c2 = q.y,
					  d1 = this->z, d2 = q.z;
				w = a1 * a2 - b1 * b2 - c1 * c2 - d1 * d2;
				x = a1 * b2 + b1 * a2 + c1 * d2 - d1 * c2;
				y = a1 * c2 - b1 * d2 + c1 * a2 + d1 * b2;
				z = a1 * d2 + b1 * c2 - c1 * b2 + d1 * a2;
			}
			template<typename T> void operator+=(T s){w += s, x += s, y += s, z += s;}
			template<typename T> void operator-=(T s){w -= s, x -= s, y -= s, z -= s;}
			template<typename T> void operator*=(T s){w *= s, x *= s, y *= s, z *= s;}
			template<typename T> void operator/=(T s){w /= s, x /= s, y /= s, z /= s;}
			inline Quaternion fromAngle(float angle, Vector3 axis){
				return Quaternion(cos(angle / 2), axis.x * sin(angle / 2), axis.y * sin(angle / 2), axis.z * sin(angle / 2));
			}
			inline Quaternion conj(){
				return (*this + Quaternion(0, 1, 0, 0)
					*(*this) * Quaternion(0, 1, 0, 0) + Quaternion(0, 0, 1, 0)
					*(*this) * Quaternion(0, 0, 1, 0) + Quaternion(0, 0, 0, 1)
					*(*this) * Quaternion(0, 0, 0, 1)) * -.5;
			}
			inline Quaternion norm(){return *this / getLength();}
			inline Quaternion recip(){return conj() / getLengthSq();}
			inline Quaternion invert(){return conj() / (getLengthSq());}
			inline float getLengthSq(){return w * w + x * x + y * y + z * z;}
			inline float getLength(){return std::sqrt(getLengthSq());}

			float w, x, y, z;
			const static Quaternion QUAT_I, QUAT_J, QUAT_K, QUAT_W;
		private:
	};

}

#endif
