#ifndef MATRIX_H
#define MATRIX_H

#include"vector.h"

namespace vb01{
	struct Matrix{
		int dim[2];
		float **matrix;
		Matrix(float**,int,int);
		Matrix(Vector3,Vector3,Vector3);
		Matrix& operator+ (Matrix&);
		Matrix& operator- (Matrix&);
		Matrix operator* (Matrix&);
		Vector3 operator* (Vector3);
		inline float* operator[](int i){return matrix[i];}
		template<typename T> Matrix& operator+ (T);
		template<typename T> Matrix& operator- (T);
		template<typename T> Matrix& operator* (T);
		template<typename T> Matrix& operator/ (T);
		void transpose();
		Matrix getSubMatrix(int,int);
		void invert();
		float getMinor(int,int);
		float getCofactor(int,int);
		float getDeterminant();
	};
}

#endif
