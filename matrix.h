#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
class Matrix
{
	protected:
    	size_t ndim;
    	size_t mdim;
    	size_t size;
	double *dataPointer;
	void diffOf(Matrix &A, Matrix &B);
	void sumOf(Matrix &A,Matrix &B);
	void split(std::vector<Matrix> subMat);
	

	public:
	inline size_t getNdim(){return ndim;}
	inline size_t getMdim(){return mdim;}
	Matrix (const int n,const int m);
	Matrix (const int n,const int m,double * datapointer);
    	Matrix (std::string filename);
	void setValue(size_t m, size_t n, double value);
   	double*createLinArray();
   	void setIdentity();
   	void print();
	void print(std::string filename);
	Matrix& operator* (Matrix MatrixB);
	static void matmult(Matrix & A, Matrix & B, Matrix & C);
};
#endif // MATRIX_H
