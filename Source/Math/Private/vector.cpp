#include "vector.h"

namespace vb01{
	const Vector2 Vector2::VEC_IJ(1,1); 
	const Vector2 Vector2::VEC_I(1,0); 
	const Vector2 Vector2::VEC_J(0,1); 
	const Vector2 Vector2::VEC_ZERO(0,0); 

	const Vector3 Vector3::VEC_IJK(1,1,1); 
	const Vector3 Vector3::VEC_I(1,0,0); 
	const Vector3 Vector3::VEC_J(0,1,0); 
	const Vector3 Vector3::VEC_K(0,0,1); 
	const Vector3 Vector3::VEC_ZERO(0,0,0); 

	const Vector4 Vector4::VEC_IJKL(1,1,1,1); 
	const Vector4 Vector4::VEC_I(1,0,0,0); 
	const Vector4 Vector4::VEC_J(0,1,0,0); 
	const Vector4 Vector4::VEC_K(0,0,1,0); 
	const Vector4 Vector4::VEC_L(0,0,0,1); 
	const Vector4 Vector4::VEC_ZERO(0,0,0,0); 
}
