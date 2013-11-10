#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
class Matrix
{
	private:
    size_t ndim;
    size_t mdim;
    size_t size;
    double *dataPointer;

	public:
    Matrix (const int n,const int m);
    Matrix (std::string filename);
	void setValue(size_t m, size_t n, double value);
    double*createLinArray();
    void setIdentity();
    void print();
	void print(std::string filename);
	Matrix& operator* (Matrix MatrixB);



};
#endif // MATRIX_H
