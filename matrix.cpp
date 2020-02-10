#include"matrix.h"
#include<iostream>

using namespace std;

namespace vb01{
	Matrix::Matrix(float **matrix,int rows,int columns){
		this->dim[0]=rows;
		this->dim[1]=columns;

		this->matrix=new float*[rows];
		for(int i=0;i<dim[0];i++){
			this->matrix[i]=new float[columns];
			for(int j=0;j<dim[1];j++)
				this->matrix[i][j]=matrix[i][j];
		}
	}

	Matrix::Matrix(Vector3 v1,Vector3 v2,Vector3 v3){
		dim[0]=3;
		dim[1]=3;

		matrix=new float*[3];

		matrix[0]=new float[3];
		matrix[0][0]=v1.x;
		matrix[0][1]=v1.y;
		matrix[0][2]=v1.z;
		matrix[1]=new float[3];
		matrix[1][0]=v2.x;
		matrix[1][1]=v2.y;
		matrix[1][2]=v2.z;
		matrix[2]=new float[3];
		matrix[2][0]=v3.x;
		matrix[2][1]=v3.y;
		matrix[2][2]=v3.z;
	}

	Matrix& Matrix::operator+(Matrix &m){
		if(dim[0]==m.dim[0]&&dim[1]==m.dim[1]){
			for(int i=0;i<dim[0];i++)
				for(int j=0;j<dim[1];j++)
					matrix[i][j]+=m.matrix[i][j];
			return *this;
		}
		else{
			cout<<"Incompatible matrix dimensions for addition.\n";
			exit(-1);
		}
	}

	Matrix& Matrix::operator-(Matrix &m){
		if(dim[0]==m.dim[0]&&dim[1]==m.dim[1]){
			for(int i=0;i<dim[0];i++)
				for(int j=0;j<dim[1];j++)
					matrix[i][j]-=m.matrix[i][j];
			return *this;
		}
		else{
			cout<<"Incompatible matrix dimensions for subtraction.\n";
			exit(-1);
		}
	}

	Matrix Matrix::operator*(Matrix &m){
		if(dim[1]==m.dim[0]){
			float **matrix=new float*[dim[0]];
			for(int i=0;i<dim[0];i++){
				matrix[i]=new float[m.dim[1]];
				for(int j=0;j<m.dim[1];j++)
					matrix[i][j]=0.;
			}

			for(int i=0;i<dim[0];i++)
				for(int j=0;j<dim[1];j++)
					for(int k=0;k<dim[0];k++)
						matrix[i][j]+=this->matrix[i][k]*m.matrix[k][j];
			return Matrix(matrix,dim[0],dim[1]);
		}
		else{
			cout<<"Incompatible matrix dimensions for multiplication.\n";
			exit(-1);
		}
	}

	Vector3 Matrix::operator* (Vector3 v){
		float **matrix=new float*[3];
		matrix[0]=new float[1];
		matrix[0][0]=v.x;
		matrix[1]=new float[1];
		matrix[1][0]=v.y;
		matrix[2]=new float[1];
		matrix[2][0]=v.z;

		Matrix vMat(matrix,3,1);
		vMat=(*this)*vMat;
		return Vector3(vMat[0][0],vMat[1][0],vMat[2][0]);
	}

	template<typename T> Matrix& Matrix::operator+(T s){
		for(int i=0;i<dim[0];i++)
			for(int j=0;j<dim[1];j++)
				matrix[i][j]+=s;
		return *this;
	}

	template<typename T> Matrix& Matrix::operator-(T s){
		for(int i=0;i<dim[0];i++)
			for(int j=0;j<dim[1];j++)
				matrix[i][j]-=s;
		return *this;
	}

	template<typename T> Matrix& Matrix::operator*(T s){
		for(int i=0;i<dim[0];i++)
			for(int j=0;j<dim[1];j++)
				matrix[i][j]*=s;
		return *this;
	}

	template<typename T> Matrix& Matrix::operator/(T s){
		for(int i=0;i<dim[0];i++)
			for(int j=0;j<dim[1];j++)
				matrix[i][j]/=s;
		return *this;
	}

	void Matrix::transpose(){
		float **matrix=new float*[dim[1]];
		for(int i=0;i<dim[1];i++)
			matrix[i]=new float[dim[0]];
		for(int i=0;i<dim[0];i++)
			for(int j=0;j<dim[1];j++)
				matrix[i][j]=this->matrix[j][i];
		this->matrix=matrix;
	}

	Matrix Matrix::getSubMatrix(int row,int column){
		int newRows=dim[0]-1,newColumns=dim[1]-1;
		float **matrix=new float*[dim[0]];
		bool offsetRows=false,offsetColumns=false;
		for(int i=0;i<dim[1];i++){
			matrix[i]=new float[dim[1]];
			for(int j=0;j<dim[1];j++)
				if(i!=row&&j!=column){
					int a=offsetRows&&i!=0?i-1:i,b=offsetColumns&&j!=0?j-1:j;
					matrix[a][b]=this->matrix[i][j];
				}
				else if(i==row) offsetRows=true;
				else if(j==column) offsetColumns=true;
		}
		return Matrix(matrix,newRows,newColumns);
	}

	void Matrix::invert(){
		transpose();
		float **matrix=new float*[dim[0]];
		for(int i=0;i<dim[0];i++){
			matrix[i]=new float[dim[1]];
			for(int j=0;j<dim[1];j++)
				matrix[i][j]=getCofactor(i,j);
		}
		this->matrix=matrix;
		*this/getDeterminant();
	}

	float Matrix::getMinor(int row,int column){
		return getSubMatrix(row,column).getDeterminant();
	}

	float Matrix::getCofactor(int row,int column){
		return ((row+column+2)%2==0?1:-1)*getMinor(row,column);	
	}

	float Matrix::getDeterminant(){
		if(dim[0]!=dim[1]){
			cout<<"Matrix not square.\n";
			exit(-1);
		}
		if(dim[0]==1)
			return matrix[0][0];
		else if(dim[0]==2)
			return matrix[0][0]*matrix[1][1]-matrix[1][0]*matrix[0][1];
		else{
			float det=0;
			for(int i=0;i<dim[0];i++)
				det+=matrix[i][0]*getCofactor(i,0);
			return det;
		}
	}
}
