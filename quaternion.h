#ifndef QUATERNION_H
#define QUATERNION_H

#include<cmath>

namespace vb01{
	class quaternion{
		public:
			quaternion(float w,float x,float y,float z){
				this->w=w;
				this->x=x;
				this->y=y;
				this->z=z;
			}
			inline vb01::quaternion operator* (vb01::quaternion q){
				float a1=this->w,a2=q.w,
					  b1=this->x,b2=q.x,
					  c1=this->y,c2=q.y,
					  d1=this->z,d2=q.z;
				float w=a1*a2-b1*b2-c1*c2-d1*d2;
				float x=a1*b2+b1*a2+c1*d2-d1*c2;
				float y=a1*c2-b1*d2+c1*a2+d1*b2;
				float z=a1*d2+b1*c2-c1*b2+d1*a2;
				return quaternion(w,x,y,z);
			}
			inline vb01::quaternion operator+(quaternion q){return quaternion(w+q.w,x+q.x,y+q.y,z+q.z);}
			inline vb01::quaternion operator-(quaternion q){return quaternion(w-q.w,x-q.x,y-q.y,z-q.z);}
			template<typename T> inline vb01::quaternion operator*(T s){return quaternion(w*s,x*s,y*s,z*s);}
			inline vb01::quaternion conj(){return (*this+vb01::quaternion(0,1,0,0)*(*this)*vb01::quaternion(0,1,0,0)+vb01::quaternion(0,0,1,0)*(*this)*vb01::quaternion(0,0,1,0)+vb01::quaternion(0,0,0,1)*(*this)*vb01::quaternion(0,0,0,1))*-.5;}
			inline vb01::quaternion operator/ (float f){return quaternion(w/f,x/f,y/f,z/f);}
			inline vb01::quaternion norm(){return *this/getLength();}
			inline vb01::quaternion recip(){return conj()/getLengthSq();}
			inline float getLengthSq(){return w*w+x*x+y*y+z*z;}
			inline float getLength(){return std::sqrt(getLengthSq());}

			float w,x,y,z;
			const static quaternion QUAT_I,QUAT_J,QUAT_K;
		private:
	};

}

#endif
