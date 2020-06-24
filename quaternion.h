#ifndef QUATERNION_H
#define QUATERNION_H

#include<cmath>
#include"vector.h"

namespace vb01{
	class Quaternion{
		public:
			Quaternion(float w,float x,float y,float z){
				this->w=w;
				this->x=x;
				this->y=y;
				this->z=z;
			}
			Quaternion(float angle, Vector3 axis){
				*this=this->fromAngle(angle,axis);
			}
			Quaternion(){}
			inline Quaternion operator- (){return Quaternion(w,-x,-y,-z);}
			inline Quaternion operator* (Quaternion q){
				float a1=this->w,a2=q.w,
					  b1=this->x,b2=q.x,
					  c1=this->y,c2=q.y,
					  d1=this->z,d2=q.z;
				float w=a1*a2-b1*b2-c1*c2-d1*d2;
				float x=a1*b2+b1*a2+c1*d2-d1*c2;
				float y=a1*c2-b1*d2+c1*a2+d1*b2;
				float z=a1*d2+b1*c2-c1*b2+d1*a2;
				return Quaternion(w,x,y,z);
			}
			inline Vector3 operator* (Vector3 v){
				Quaternion vQuat=(*this)*Quaternion(0,v.x,v.y,v.z)*recip();
				return Vector3(vQuat.x,vQuat.y,vQuat.z);
			}
			inline Quaternion fromAngle(float angle, Vector3 axis){
				return Quaternion(cos(angle/2),axis.x*sin(angle/2),axis.y*sin(angle/2),axis.z*sin(angle/2));
			}
			inline float getAngle(){return 2*acos(w);}
			inline Vector3 getAxis(){return getAngle()==0?Vector3(1,0,0):Vector3(x,y,z).norm();}
			inline Quaternion operator+(Quaternion q){return Quaternion(w+q.w,x+q.x,y+q.y,z+q.z);}
			inline Quaternion operator-(Quaternion q){return Quaternion(w-q.w,x-q.x,y-q.y,z-q.z);}
			template<typename T> inline Quaternion operator*(T s){return Quaternion(w*s,x*s,y*s,z*s);}
			template<typename T> inline Quaternion operator/(T s){return Quaternion(w/s,x/s,y/s,z/s);}
			inline Quaternion conj(){
				return (*this+Quaternion(0,1,0,0)
					*(*this)*Quaternion(0,1,0,0)+Quaternion(0,0,1,0)
					*(*this)*Quaternion(0,0,1,0)+Quaternion(0,0,0,1)
					*(*this)*Quaternion(0,0,0,1))*-.5;
			}
			inline Quaternion norm(){return *this/getLength();}
			inline Quaternion recip(){return conj()/getLengthSq();}
			inline Quaternion invert(){return conj()/(getLengthSq());}
			inline float getLengthSq(){return w*w+x*x+y*y+z*z;}
			inline float getLength(){return std::sqrt(getLengthSq());}

			float w,x,y,z;
			const static Quaternion QUAT_I,QUAT_J,QUAT_K,QUAT_W;
		private:
	};

}

#endif
